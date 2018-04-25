#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/DAC.h"

void DAC_Init(void){
    SYSCTL_RCGC2_R |= 0x02;
    while ((SYSCTL_PRGPIO_R & 0x02) != 0x02) ;
    GPIO_PORTB_DIR_R |= 0x3F;
    GPIO_PORTB_AFSEL_R &= ~0x3F;
    GPIO_PORTB_DEN_R |= 0x3F;
}

void DAC_Out(uint32_t data){
    GPIO_PORTB_DATA_R = data;
}
