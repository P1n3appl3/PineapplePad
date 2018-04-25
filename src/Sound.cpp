// Sound.cpp
// Runs on any computer
// Sound assets based off the original Space Invaders
// Import these constants into your SpaceInvaders.c for sounds!
// Jonathan Valvano
// April 19, 2018
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/Sound.h"
#include "../inc/DAC.h"

extern "C" void SysTick_Handler(void);

uint32_t sample = 0;
uint32_t limit;
uint8_t* currentBuf;

void SysTick_Handler(void){
    DAC_Out(currentBuf[sample++]>>2);
    if(sample == limit){
        Sound_Stop();
    }
}

void SysTick_Init() {
    NVIC_ST_CTRL_R = 0;
    NVIC_ST_CURRENT_R = 0;
    NVIC_ST_RELOAD_R = 0;
    NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x40000000; // priority 2
    NVIC_ST_CTRL_R = 0x07;
}

void Sound_Init(void){
    SysTick_Init();
    DAC_Init();
}

#define AUDIO_FREQ (80 * 1000 * 1000 / 11000) // 11 khz
void Sound_Play(const uint8_t* pt, uint32_t count){
    sample = 0;
    currentBuf = (uint8_t*)pt; // casting away const
    limit = count;
    NVIC_ST_RELOAD_R = AUDIO_FREQ;
}

void Sound_Stop(){
    NVIC_ST_RELOAD_R = 0;
}
