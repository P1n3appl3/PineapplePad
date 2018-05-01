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

void PortF_Init(void) {
    SYSCTL_RCGCGPIO_R |= 0x20;
    while ((SYSCTL_PRGPIO_R & 0x20) != 0x20) ;
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = 0x1F;
    GPIO_PORTF_AMSEL_R = 0x00;
    GPIO_PORTF_PCTL_R = 0x00000000;
    GPIO_PORTF_DIR_R = 0x0E;
    GPIO_PORTF_AFSEL_R = 0x00;
    GPIO_PORTF_PUR_R = 0x11;
    GPIO_PORTF_DEN_R = 0x1F;
}

SlidePot pot(1500, 0);

extern "C" void DisableInterrupts(void);
extern "C" void EnableInterrupts(void);

int main(void){
    PLL_Init(Bus80MHz);
    Sound_Init();
    PortF_Init();
    ST7735_InitR(INITR_REDTAB);
    Random_Init(1);
    UART_Init();
    initField();
    Timer1_Init(step, 1333333); // 60 Hz
    EnableInterrupts();

    // display title screen and wait for press
    Random_Init(NVIC_ST_CURRENT_R);

    while (1) {
        drawPlayer();
        for (int i = 0; i < NUM_CUBES; ++i) {
            cubes[i].draw();
        }
        frameDone = false;
        difficulty = 1 + (pot.ADCsample() >> 9);
        // chill until new calculations... maybe load some audio?
        while (!frameDone) ;
        ST7735_FillRect(0, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, bgColor);
        vel = (!(GPIO_PORTF_DATA_R & 0x10))
              - (!(GPIO_PORTF_DATA_R & 0x01));
        if (!(GPIO_PORTF_DATA_R & 0x11)) {
            shuffleColor();
        }
    }
}
