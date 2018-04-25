// main.cpp
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the EE319K Lab 10 in C++

// Last Modified: 4/19/2018
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2017

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2017

   Copyright 2018 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
   THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
   OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
   VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
   OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
   For more information about my classes, my research, and my books, see
   http://users.ece.utexas.edu/~valvano/
 */
// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PD2/AIN5
// Slide pot pin 3 connected to +3.3V
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

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

#define PF0  (*((volatile uint32_t*)0x40025004))  // right BTN
#define PF1  (*((volatile uint32_t*)0x40025008))  // red LED
#define PF2  (*((volatile uint32_t*)0x40025010))  // blue LED
#define PF3  (*((volatile uint32_t*)0x40025020))  // green LED
#define PF4  (*((volatile uint32_t*)0x40025040))  // left BTN

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

void noise(void){
    // static int i = 0;
    // DAC_Out((++i) %= 1 << 6);
    //GPIO_PORTF_DATA_R ^= 0x02;
}

void clock(void){
    //GPIO_PORTF_DATA_R ^= 0x04;
    Sound_Play(explosion, sizeof(explosion));
}

int main(void){
    PLL_Init(Bus80MHz);     // Bus clock is 80 MHz
    Random_Init(1);
    //Output_Init();
    Sound_Init();
    PortF_Init();
    Timer0_Init(&noise, 160000); // 500 Hz
    Timer1_Init(&clock, 80000000); // 1 Hz
    EnableInterrupts();
    //ST7735_FillScreen(0x0000);          // set screen to black
    while (1) {
        if (GPIO_PORTF_DATA_R & 0x10) {
            GPIO_PORTF_DATA_R &= ~0x08;
        }else {
            GPIO_PORTF_DATA_R |= 0x08;
        }
    }
}
