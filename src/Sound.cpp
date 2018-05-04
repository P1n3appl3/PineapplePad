#include <stdint.h>
#include "../inc/dac.h"
#include "../inc/sound.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/IO.h"

const uint8_t* instruments[] = { horn, guitar, basoon, sine, flute };

const uint8_t* instrument;

void Timer0A_Init(void){
    SYSCTL_RCGCTIMER_R |= 0x01;
    TIMER0_CTL_R = 0;
    TIMER0_CFG_R = 0;
    TIMER0_TAMR_R = 0x02;
    TIMER0_TAPR_R = 0;
    TIMER0_ICR_R = 0x01;
    TIMER0_IMR_R = 0x01;
    NVIC_PRI4_R = (NVIC_PRI4_R & 0x00FFFFFF) | 0x40000000;
    NVIC_EN0_R = 1 << 19;
    TIMER0_CTL_R = 1;
}

void Sound_Init(void){
    Timer0A_Init();
}

extern "C" void TIMER0A_Handler(void);

void TIMER0A_Handler(void){
    TIMER0_ICR_R = TIMER_ICR_TATOCINT;
    static uint8_t i = 0;
    DAC_Out(instrument[i++]);
    i &= 0x3F;
}

void Sound_Play(uint32_t period){
    if (!period) {
        TIMER0_TAILR_R = 0;
    }else {
        TIMER0_TAILR_R = period - 1;
    }
}

void Sound_Stop(){
    Sound_Play(0);
}
