#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/PLL.h"
#include "../inc/ST7735.h"
#include "../inc/random.h"
#include "../inc/SlidePot.h"
#include "../inc/UART.h"
#include "../inc/Timer0.h"
#include "../inc/Timer1.h"
#include "../inc/Sound.h"
#include "../inc/graphics.h"
#include "../inc/game.h"
#include "../inc/IO.h"

extern "C" void DisableInterrupts(void);
extern "C" void EnableInterrupts(void);

extern void Delay1ms(uint32_t n);

int main(void){
    PLL_Init(Bus80MHz);     // Bus clock is 80 MHz
    ST7735_InitR(INITR_REDTAB);
    Sound_Init();
    Random_Init(NVIC_ST_CURRENT_R);
    UART_Init();            // debugging
    SD_Mount();
    EnableInterrupts();
    Sound_Play();
    DisableInterrupts();
    Delay1ms(1);
    initField();            // Set up cubes
    Delay1ms(1);
    PortF_Init();
    Timer1_Init(step, 1333333); // 60 FPS
    ADC_Init();
    EnableInterrupts();

    // display title screen and wait for press
    while (1) {
        frameDone = false;
        difficulty = 1 + (ADC_In() >> 9); // slide pot input
        while (!frameDone){
            DisableInterrupts();
            Sound_Load();
            EnableInterrupts();
        }
        vel = (!(GPIO_PORTF_DATA_R & 0x10)) // replace with hardware interrupts
              - (!(GPIO_PORTF_DATA_R & 0x01));
        if (!(GPIO_PORTF_DATA_R & 0x11)) {
            shuffleColor();
        }
    }
}

int main1(){
    PLL_Init(Bus80MHz);
    ST7735_InitR(INITR_REDTAB);
    Sound_Init();
    UART_Init();
    PortF_Init();
    EnableInterrupts();
    Random_Init(NVIC_ST_CURRENT_R);
    SD_Mount();

    Sound_Play();
    while(1){
        Sound_Load();
    };
    //for(int i=0; i<1000; i++){
     //   Sound_Test();
    //    Sound_Load();
    //}
    //Sound_Stop();
}

int main2(void){
    PLL_Init(Bus80MHz);
    ST7735_InitR(INITR_REDTAB);
    EnableInterrupts();
    sd_test();
    return 0;
}
