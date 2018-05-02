#ifndef __UART_H__
#define __UART_H__
#include <stdint.h>
// standard ASCII symbols
#define STX  0x02
#define ETX  0x03
#define CR   0x0D
#define LF   0x0A
#define BS   0x08
#define ESC  0x1B
#define SP   0x20
#define DEL  0x7F

//------------UART_Init------------
// Initialize the UART for 115,200 baud rate (assuming 50 MHz clock),
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// Input: none
// Output: none
void UART_Init(void);

//------------UART_InChar------------
// Wait for new serial port input
// Input: none
// Output: ASCII code for key typed
char UART_InChar(void);

//------------UART_InStatus------------
// check for possible input ready
// Input: none
// Output: true if some input is ready
bool UART_InStatus(void);

//------------UART_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART_OutChar(char data);

//------------UART_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART_OutString(char *pt);

//-----------------------UART_OutUDec-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
void UART_OutUDec(uint32_t n);

//------------UART_InMessage------------
// Accepts ASCII characters from the serial port
//    and adds them to a string until ETX is typed
//    or until max length of the string is reached.
// Input: pointer to empty buffer of 8 characters
// Output: Null terminated string
void UART_InMessage(char *bufPt);
#endif
