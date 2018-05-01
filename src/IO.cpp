#include "../inc/tm4c123gh6pm.h"
#include "../inc/IO.h"
#include <stdint.h>

#define NVIC_EN0_INT4 0x10
#define debounce 80000 // 1ms

int time1;
int time2;

void IO_Init(void){
    SYSCTL_RCGCGPIO_R |= 0x10;          //activate PortE clock
    while ((SYSCTL_PRGPIO_R & 0x10) != 0x10) ; //wait for clock
    GPIO_PORTE_DIR_R &= ~0x03;          //set E0, E1 output
    GPIO_PORTE_AFSEL_R &= ~0x03;        //disable alt functions
    GPIO_PORTE_DEN_R |= 0x03;           //enable digital
    GPIO_PORTE_AMSEL_R &= ~0x03;        //disable analog functionality
    GPIO_PORTE_PUR_R |= 0x03;           //enable weak pull-up on E0,E1
    GPIO_PORTE_IS_R &= ~0x03;           //E0, E1 edge sensitive
    GPIO_PORTE_IBE_R |= 0x03;           //both edges
    GPIO_PORTE_ICR_R = 0x03;            //clear flags
    GPIO_PORTE_IM_R |= 0x03;            //arm interrupts
    NVIC_PRI1_R = (NVIC_PRI1_R & NVIC_PRI1_INT4_M) | 0xA0; //priority 5
    NVIC_EN0_R |= NVIC_EN0_INT4;      //enable interrupt 4 in NVIC
    time1 = NVIC_ST_CURRENT_R;
    time2 = NVIC_ST_CURRENT_R;
    button1 = false;
    button1 = false;
}

void GPIOE_Handler(void){
    int current_time = NVIC_ST_CURRENT_R;
    if (current_time > time1 && (current_time - time2 > debounce)) {
        if (GPIO_PORTE_RIS_R & 0x01) {
            button1 = !button1;
            time1 = NVIC_ST_CURRENT_R;
            GPIO_PORTE_ICR_R = 0x01;
        }
    }
    if (current_time > time2 && (current_time - time2 > debounce)) {
        if (GPIO_PORTE_RIS_R & 0x02) {
            button1 = !button1;
            time2 = NVIC_ST_CURRENT_R;
            GPIO_PORTE_ICR_R = 0x02;
        }
    }
}

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
