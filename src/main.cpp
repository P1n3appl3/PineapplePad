#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/PLL.h"
#include "../inc/ST7735.h"
#include "../inc/random.h"
#include "../inc/ADC.h"
#include "../inc/UART.h"
#include "../inc/Timer0.h"
#include "../inc/Timer1.h"
#include "../inc/sound.h"
#include "../inc/sfx.h"
#include "../inc/graphics.h"
#include "../inc/game.h"
#include "../inc/IO.h"
#include "../inc/Images.h"

extern "C" void DisableInterrupts(void);
extern "C" void EnableInterrupts(void);

volatile bool frameDone;

void clk(){
    if (frameDone) { // too high fps
        //toggle_green();
        //while (1) ;
    }
    frameDone = true;
}

int main(void){
    PLL_Init(Bus80MHz);
    ST7735_InitR(INITR_REDTAB);
    UART_Init();    // debugging
    ADC_Init();
    initField();    // set up cubes
    PortF_Init();
    Sound_Init();

    // SD_Mount();
    // Sound_Play();

    EnableInterrupts();

    ST7735_DrawBitmap(0, 159, retro, 128, 160);
    while((!(GPIO_PORTF_DATA_R & 0x10)) - (!(GPIO_PORTF_DATA_R & 0x01)) == 0);
    ST7735_FillScreen(0);
    Timer1_Init(clk, 3333333); // 24 FPS
    Random_Init(NVIC_ST_CURRENT_R);

    while (1) {
        frameDone = false;
        difficulty = 1 + (ADC_In() >> 9); // slide pot input
        vel = (!(GPIO_PORTF_DATA_R & 0x10)) // todo: replace with hardware interrupts
              - (!(GPIO_PORTF_DATA_R & 0x01));
        step();
        do {
            //Sound_Load();
        } while (!frameDone);
    }
}

int mainSFX(){
    PLL_Init(Bus80MHz);
    PortF_Init();
    Sound_Init();
    Effect_Play(dead, sizeof(dead));
    while (1) ;
}

int mainAudio(){
    PLL_Init(Bus80MHz);
    ST7735_InitR(INITR_REDTAB);
    Sound_Init();
    UART_Init();
    PortF_Init();
    EnableInterrupts();
    Random_Init(NVIC_ST_CURRENT_R);
    SD_Mount();

    //Sound_Load();

    Sound_Play();
    while (1) {
        //Sound_Load();
    }
    //for(int i=0; i<1000; i++){
    //   Sound_Test();
    //    Sound_Load();
    //}
    //Sound_Stop();
}

int mainSD(void){
    PLL_Init(Bus80MHz);
    ST7735_InitR(INITR_REDTAB);
    EnableInterrupts();
    sd_test();
    while (1) ;
}
