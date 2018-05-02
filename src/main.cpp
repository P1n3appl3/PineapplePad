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

int main(void){
    PLL_Init(Bus80MHz);     // Bus clock is 80 MHz
    Sound_Init();           // Start up systick for music
    ST7735_InitR(INITR_REDTAB);
    UART_Init();            // debugging
    initField();            // Set up cubes
    PortF_Init();
    Timer1_Init(step, 1333333); // 60 FPS
    ADC_Init();
    EnableInterrupts();

    // display title screen and wait for press
    Random_Init(NVIC_ST_CURRENT_R);

    while (1) {
        frameDone = false;
        difficulty = 1 + (ADC_In() >> 9);
        // chill until new calculations... maybe load some audio?
        while (!frameDone) ;
        vel = (!(GPIO_PORTF_DATA_R & 0x10))
              - (!(GPIO_PORTF_DATA_R & 0x01));
        if (!(GPIO_PORTF_DATA_R & 0x11)) {
            shuffleColor();
        }
    }
}
