#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/PLL.h"
#include "../inc/random.h"
#include "../inc/ADC.h"
#include "../inc/DAC.h"
#include "../inc/UART.h"
#include "../inc/Timer1.h"
#include "../inc/ST7735.h"
#include "../inc/graphics.h"
#include "../inc/game.h"
#include "../inc/IO.h"
#include "../inc/Images.h"
#include "../inc/sound.h"
#include "../inc/song.h"

extern "C" void DisableInterrupts(void);
extern "C" void EnableInterrupts(void);

volatile bool frameDone;

void clk(){
    if (frameDone) { // fps too high
        //toggle_red();
    }
    frameDone = true;
}

int main(void){
    PLL_Init(Bus80MHz);
    ST7735_InitR(INITR_REDTAB);
    UART_Init();    // debugging
    ADC_Init();
    PortF_Init();
    DAC_Init();
    Sound_Init();
    Song_Init();

    EnableInterrupts();

    ST7735_DrawBitmap(0, 159, retro, 128, 160);
    //Song_Play(menu_pitch, menu_dur, menu_speed, sizeof(menu_dur));
    Song_Play(megaman_pitch, megaman_dur, megaman_speed, sizeof(megaman_dur));
    while ((GPIO_PORTF_DATA_R & 0x10) && (GPIO_PORTF_DATA_R & 0x01)) ;
    Random_Init(NVIC_ST_CURRENT_R);
    instrument = instruments[Random() % sizeof(instruments)];
    Song_Play(tears_pitch, tears_dur, tears_speed, sizeof(tears_dur));
    ST7735_FillScreen(0);
    printScore(0);
    initField(); // set up cubes
    Timer1_Init(clk, 1333333); // 60 FPS

    while (1) {
        frameDone = false;
        difficulty = 1 + (ADC_In() >> 9); // slide pot input
        vel = (!(GPIO_PORTF_DATA_R & 0x10)) // todo: replace with hardware interrupts
              - (!(GPIO_PORTF_DATA_R & 0x01));
        step();
        while (!frameDone) ;
    }
}

int mainSound(){
    DisableInterrupts();
    PLL_Init(Bus80MHz);
    DAC_Init();
    Sound_Init();
    Song_Init();
    PortF_Init();
    EnableInterrupts();
    Song_Play(tears_pitch, tears_dur, tears_speed, sizeof(tears_dur));
    while (1) {
    }
}
