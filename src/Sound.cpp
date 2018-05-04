#include <stdint.h>
#include "../inc/dac.h"
#include "../inc/sound.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/IO.h"

const int horn[64] = {
  63,64,65,66,68,74,81,98,114,117,120,
  101,81,59,38,34,30,30,30,31,33,34,
  36,37,39,42,45,53,61,65,69,72,
  75,81,86,91,97,100,103,98,94,100,
  107,98,89,77,65,49,34,26,18,15,
  12,14,16,19,22,27,32,37,42,49,57,60
};

void Timer0A_Init(void){
    SYSCTL_RCGCTIMER_R |= 0x01; // activate
    TIMER0_CTL_R = 0; // disable during setup
    TIMER0_CFG_R = 0; // configure for 32-bit mode
    TIMER0_TAMR_R = 0x02; // configure for periodic mode
    TIMER0_TAPR_R = 0; // bus clock resolution
    TIMER0_ICR_R = 0x01; // clear timeout flag
    TIMER0_IMR_R = 0x01; // arm timeout interrupt
    NVIC_PRI4_R = (NVIC_PRI4_R & 0x00FFFFFF) | 0x40000000; // priority 2
    NVIC_EN0_R = 1 << 19; // enable IRQ 19 in NVIC
    TIMER0_CTL_R = 1;
}

void Sound_Init(void){
    Timer0A_Init();
}

void TIMER0A_Handler(void){
    TIMER0_ICR_R = TIMER_ICR_TATOCINT;
    static char i = 0;
	toggle_blue();
    DAC_Out(horn[i++]);
    i &= 0x3F;
}

void Sound_Play(uint32_t period){
    if (!period) {
        TIMER0_TAILR_R = 0;
    }else  {
        TIMER0_TAILR_R = period - 1;
    }
}

void Sound_Stop(){
	Sound_Play(0);
}

void Effect_Play(const uint8_t* pt, uint32_t count){
		// redo
    NVIC_ST_CTRL_R |= 0x02; // enable interrupt
}
