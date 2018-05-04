#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/PLL.h"
#include "../inc/ST7735.h"
#include "../inc/random.h"
#include "../inc/ADC.h"
#include "../inc/DAC.h"
#include "../inc/UART.h"
#include "../inc/Timer1.h"
#include "../inc/sound.h"
#include "../inc/sfx.h"
#include "../inc/graphics.h"
#include "../inc/game.h"
#include "../inc/IO.h"
#include "../inc/Images.h"
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
    while ((GPIO_PORTF_DATA_R & 0x10) && (GPIO_PORTF_DATA_R & 0x01)) ;
    //Sound_Stop();
    //Effect_Play(start, sizeof(start));
    Song_Play();

    ST7735_FillScreen(0);
    printScore(0);
    initField();    // set up cubes
    Timer1_Init(clk, 1333333); // 60 FPS
    //Random_Init(NVIC_ST_CURRENT_R);

    while (1) {
        frameDone = false;
        difficulty = 1 + (ADC_In() >> 9); // slide pot input
        vel = (!(GPIO_PORTF_DATA_R & 0x10)) // todo: replace with hardware interrupts
              - (!(GPIO_PORTF_DATA_R & 0x01));
        step();
				while (!frameDone);
    }
}

int mainSFX(){
    PLL_Init(Bus80MHz);
    PortF_Init();
    Sound_Init();
    Effect_Play(death, sizeof(death));
    while (1) ;
}

int main1(){
    DisableInterrupts();
    PLL_Init(Bus80MHz);
	DAC_Init();
    Sound_Init();
	Song_Init();
    PortF_Init();
    EnableInterrupts();
    Song_Play();
    while (1) {
			
    }
}
