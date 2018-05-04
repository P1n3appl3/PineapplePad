#include "../inc/tm4c123gh6pm.h"
#include "../inc/IO.h"
#include <stdint.h>

void PortF_Init(void) {
    SYSCTL_RCGCGPIO_R |= 0x20;
    while ((SYSCTL_PRGPIO_R & 0x20) != 0x20) ;
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = 0x1F;
    GPIO_PORTF_AMSEL_R = 0x00;
    GPIO_PORTF_DIR_R = 0x0E;
    GPIO_PORTF_AFSEL_R = 0x00;
    GPIO_PORTF_PUR_R = 0x11;
    GPIO_PORTF_DEN_R = 0x1F;
}

void toggle_red(void){
    GPIO_PORTF_DATA_R ^= 0x02;
}

void toggle_blue(void) {
    GPIO_PORTF_DATA_R ^= 0x04;
}

void toggle_green(void){
    GPIO_PORTF_DATA_R ^= 0x08;
}
