#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/UART.h"

#define PF1  (*((volatile uint32_t*)0x40025008))

extern "C" void DisableInterrupts(void); // Disable interrupts
extern "C" long StartCritical(void);     // previous I bit, disable interrupts
extern "C" void EndCritical(long sr);    // restore I bit to previous value
extern "C" void WaitForInterrupt(void);  // low power mode
extern "C" void EnableInterrupts(void);

#define NVIC_EN0_INT5    0x00000020  // Interrupt 5 enable
extern "C" void UART0_Handler(void);
#define TXFIFOSIZE 32 // must be a power of 2
#define FIFOSIZE 32
#define FIFOSUCCESS 1
#define FIFOFAIL    0

typedef char txDataType;
uint32_t volatile TxPutI;// put next
uint32_t volatile TxGetI;// get next
txDataType static TxFifo[TXFIFOSIZE];

// initialize index FIFO
void TxFifo_Init(void) {
    long sr;
    sr = StartCritical(); // make atomic
    TxPutI = TxGetI = 0;  // Empty
    EndCritical(sr);
}

// add element to end of index FIFO
// return TXFIFOSUCCESS if successful
int TxFifo_Put(txDataType data) {
    if (((TxPutI + 1) & (TXFIFOSIZE - 1)) == TxGetI) {
        return(FIFOFAIL); // Failed, fifo full
    }
    TxFifo[TxPutI] = data; // put
    TxPutI = (TxPutI + 1) & (TXFIFOSIZE - 1); // Success, update
    return(FIFOSUCCESS);
}

// remove element from front of index FIFO
// return TXFIFOSUCCESS if successful
int TxFifo_Get(txDataType* datapt) {
    if (TxPutI == TxGetI) {
        return(FIFOFAIL); // Empty if TxPutI=TxGetI
    }
    *datapt = TxFifo[TxGetI];
    TxGetI = (TxGetI + 1) & (TXFIFOSIZE - 1); // Success, update
    return(FIFOSUCCESS);
}

// number of elements in index FIFO
// 0 to TXFIFOSIZE-1
uint32_t TxFifo_Size(void) {
    return (TxPutI - TxGetI) & (TXFIFOSIZE - 1);
}

#define RXFIFOSIZE 32 // must be a power of 2
typedef char rxDataType;
uint32_t volatile RxPutI;// put next
uint32_t volatile RxGetI;// get next
rxDataType static RxFifo[RXFIFOSIZE];
// initialize index FIFO
void RxFifo_Init(void) {
    long sr;
    sr = StartCritical(); // make atomic
    RxPutI = RxGetI = 0;  // Empty
    EndCritical(sr);
}

// add element to end of index FIFO
// return FIFOSUCCESS if successful
int RxFifo_Put(rxDataType data) {
    if (((RxPutI + 1) & (RXFIFOSIZE - 1)) == RxGetI) {
        return(FIFOFAIL); // Failed, fifo full
    }
    RxFifo[RxPutI] = data; // put
    RxPutI = (RxPutI + 1) & (RXFIFOSIZE - 1); // Success, update
    return(FIFOSUCCESS);
}

// remove element from front of index FIFO
// return FIFOSUCCESS if successful
int RxFifo_Get(rxDataType* datapt) {
    if (RxPutI == RxGetI) {
        return(FIFOFAIL); // Empty if TxPutI=TxGetI
    }
    *datapt = RxFifo[RxGetI];
    RxGetI = (RxGetI + 1) & (RXFIFOSIZE - 1); // Success, update
    return(FIFOSUCCESS);
}

// number of elements in index FIFO
// 0 to RXFIFOSIZE-1
uint32_t RxFifo_Size(void) {
    return (RxPutI - RxGetI) & (RXFIFOSIZE - 1);
}

// Initialize UART0
// Baud rate is 115200 bits/sec
void UART_Init(void) {
    SYSCTL_RCGCUART_R |= 0x01;            // activate UART0
    SYSCTL_RCGCGPIO_R |= 0x01;            // activate port A
    RxFifo_Init();                        // initialize empty FIFOs
    TxFifo_Init();
    UART0_CTL_R &= ~UART_CTL_UARTEN;      // disable UART
    UART0_IBRD_R =
        43;                    // IBRD = int(80,000,000 / (16 * 115,200)) = int(43.403)
    UART0_FBRD_R = 26;                    // FBRD = round(0.403 * 64 ) = 26
    // 8 bit word length (no parity bits, one stop bit, FIFOs)
    UART0_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);
    UART0_IFLS_R &=
        ~0x3F;                // clear TX and RX interrupt FIFO level fields
    // configure interrupt for TX FIFO <= 1/8 full
    // configure interrupt for RX FIFO >= 1/8 full
    UART0_IFLS_R += (UART_IFLS_TX1_8 | UART_IFLS_RX1_8);
    // enable TX and RX FIFO interrupts and RX time-out interrupt
    UART0_IM_R |= (UART_IM_RXIM | UART_IM_TXIM | UART_IM_RTIM);
    UART0_CTL_R |= 0x301;                 // enable UART
    GPIO_PORTA_AFSEL_R |= 0x03;           // enable alt funct on PA1-0
    GPIO_PORTA_DEN_R |= 0x03;             // enable digital I/O on PA1-0
    // configure PA1-0 as UART
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFFFFFF00) + 0x00000011;
    // UART0=priority 2
    NVIC_PRI1_R = (NVIC_PRI1_R & 0xFFFF00FF) | 0x00004000; // bits 13-15
    NVIC_EN0_R = NVIC_EN0_INT5;           // enable interrupt 5 in NVIC
}

// copy from hardware RX FIFO to software RX FIFO
// stop when hardware RX FIFO is empty or software RX FIFO is full
void static copyHardwareToSoftware(void) {
    char letter;
    while (((UART0_FR_R & UART_FR_RXFE) == 0) && (RxFifo_Size() < (FIFOSIZE - 1))) {
        letter = UART0_DR_R;
        RxFifo_Put(letter);
    }
}

// copy from software TX FIFO to hardware TX FIFO
// stop when software TX FIFO is empty or hardware TX FIFO is full
void static copySoftwareToHardware(void) {
    char letter;
    while (((UART0_FR_R & UART_FR_TXFF) == 0) && (TxFifo_Size() > 0)) {
        TxFifo_Get(&letter);
        UART0_DR_R = letter;
    }
}

// input ASCII character from UART
// spin if RxFifo is empty
char UART_InChar(void) {
    char letter;
    while (RxFifo_Get(&letter)) {
    }
    ;
    return(letter);
}

bool UART_InStatus(void) {
    // true if input data ready
    return RxFifo_Size() > 0;
}

// output ASCII character to UART
// spin if TxFifo is full
void UART_OutChar(char data) {
    while (TxFifo_Put(data) == FIFOFAIL) {
    }
    ;
    UART0_IM_R &= ~UART_IM_TXIM;          // disable TX FIFO interrupt
    copySoftwareToHardware();
    UART0_IM_R |= UART_IM_TXIM;           // enable TX FIFO interrupt
}

// at least one of three things has happened:
// hardware TX FIFO goes from 3 to 2 or less items
// hardware RX FIFO goes from 1 to 2 or more items
// UART receiver has timed out
void UART0_Handler(void) {
    if (UART0_RIS_R & UART_RIS_TXRIS) {    // hardware TX FIFO <= 2 items
        UART0_ICR_R = UART_ICR_TXIC;        // acknowledge TX FIFO
        // copy from software TX FIFO to hardware TX FIFO
        copySoftwareToHardware();
        if (TxFifo_Size() == 0) {            // software TX FIFO is empty
            UART0_IM_R &= ~UART_IM_TXIM;      // disable TX FIFO interrupt
        }
    }
    if (UART0_RIS_R & UART_RIS_RXRIS) {    // hardware RX FIFO >= 2 items
        UART0_ICR_R = UART_ICR_RXIC;        // acknowledge RX FIFO
        // copy from hardware RX FIFO to software RX FIFO
        copyHardwareToSoftware();
    }
    if (UART0_RIS_R & UART_RIS_RTRIS) {    // receiver timed out
        UART0_ICR_R = UART_ICR_RTIC;        // acknowledge receiver time out
        // copy from hardware RX FIFO to software RX FIFO
        copyHardwareToSoftware();
    }
}

//------------UART_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART_OutString(char* pt) {
    while (*pt) {
        UART_OutChar(*pt);
        pt++;
    }
}

//-----------------------UART_OutUDec-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
void UART_OutUDec(uint32_t n) {
// This function uses recursion to convert decimal number
//   of unspecified length as an ASCII string
    if (n >= 10) {
        UART_OutUDec(n / 10);
        n = n % 10;
    }
    UART_OutChar(n + '0'); /* n is between 0 and 9 */
}

//------------UART_InMessage------------
// Accepts ASCII characters from the serial port
//    and adds them to a string until ETX is typed
//    or until max length of the string is reached.
// Input: pointer to empty buffer of 8 characters
// Output: Null terminated string
void UART_InMessage(char* bufPt) {
    for (int i = 0; i < 8; ++i) {
        char in = UART_InChar();
        bufPt[i] = in;
        if (in == ETX) {
            break;
        }
    }
}
