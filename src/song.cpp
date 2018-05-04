#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/song.h"
#include "../inc/sound.h"
#include "../inc/IO.h"

uint16_t note;
uint32_t counter;

const uint16_t* pitch;
const uint8_t* dur;
uint16_t limit;

void SysTick_Init(void){
    NVIC_ST_CTRL_R = 0;
    NVIC_ST_CURRENT_R = 0;
    NVIC_ST_RELOAD_R = 600000;
    NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x40000000; // priority 2
    NVIC_ST_CTRL_R = 0x06; // core clock and interrupts
}

void Song_Init(void){
    SysTick_Init();
    PortF_Init();
}

void Song_Play(const uint16_t* pitches, const uint8_t* durs, uint32_t speed, uint16_t len){
    note = 0;
    pitch = pitches;
    dur = durs;
    limit = len;
    NVIC_ST_RELOAD_R = speed;
    counter = dur[note];
    Sound_Play(pitch[note]);
    NVIC_ST_CTRL_R |= 0x01;
}

void Song_Stop(void){
    NVIC_ST_CTRL_R &= ~0x01;
    Sound_Play(0);
}

extern "C" void SysTick_Handler(void);

void SysTick_Handler(void){
    if (--counter) {
        return;
    }
    if (++note == limit) {
        Song_Stop();
        return;
    }
    Sound_Play(pitch[note]);
    counter = dur[note];
    // toggle_red();
}
