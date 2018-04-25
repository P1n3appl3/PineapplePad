#include <stdint.h>
#include "../inc/SlidePot.h"
#include "../inc/tm4c123gh6pm.h"

// ADC initialization function
// Input: none
// Output: none
// measures from PD2, analog channel 5
void ADC_Init(void) {
    SYSCTL_RCGCGPIO_R |= 0x08; // Turn on port d clock
    for(volatile int dummy = 0; dummy < 0x1337; dummy++) {}
    GPIO_PORTD_DIR_R &= ~(1 << 2);  // Set to input
    GPIO_PORTD_AFSEL_R |= 1 << 2;   // Turn on alternate func
    GPIO_PORTD_DEN_R &= ~(1 << 2);  // Turn of digital
    GPIO_PORTD_AMSEL_R |= 1 << 2;   // Turn on analog
    SYSCTL_RCGC0_R |= 1 << 16;      // Enable adc clock
    for(volatile int dummy = 0; dummy < 0x1337; dummy++) {}
    SYSCTL_RCGC0_R &= ~0x00000300;  // Set to lowest speed
    ADC0_SSPRI_R = 0x0123;          // Set sequencer priorities
    ADC0_ACTSS_R &= ~0x0008;        // Disable sequencer
    ADC0_EMUX_R &= ~0xF000;         // Set software trigger for seq3
    ADC0_SSMUX3_R &= ~0x000F;       // Deselect analog channels
    ADC0_SSMUX3_R += 5;             // Analog channel 5 = PD2
    ADC0_SSCTL3_R = 0x0006;         // Magical control bits
    ADC0_ACTSS_R |= 0x0008;         // Re-enable adc
}

//------------ADCIn------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
// measures from PD2, analog channel 5
uint32_t ADC_In(void) {
    ADC0_PSSI_R |= 0x08;            // Initiate ADC poll
    while(!(ADC0_RIS_R&0x08));      // Wait for it to finish
    uint32_t temp = ADC0_SSFIFO3_R & 0x0FFF; // Get result
    ADC0_ISC_R |= 0x08;             // Ack
    return temp;
}

// constructor, invoked on creation of class
// m and b are linear calibration coeffients
SlidePot::SlidePot(uint32_t m, uint32_t b) {
        slope = m;
        offset = b;
}

void SlidePot::Save(uint32_t n) {
        data = n;
        distance = Convert(n);
        flag = true;
}
//todo: replace these with real values measured from part D
#define NUMERATOR 58
#define DENOMINATOR 125
#define YINT 2024
uint32_t SlidePot::Convert(uint32_t n) {
    // use calibration data to convert ADC sample to distance
    return YINT - (n * NUMERATOR/DENOMINATOR);
}

void SlidePot::Sync(void) {
        while(!flag);
        flag = 0;
}

uint32_t SlidePot::ADCsample(void) { // return ADC sample value (0 to 4095)
    return data;
}

uint32_t SlidePot::Distance(void) { // return distance value (0 to 2000), 0.001cm
    return distance;
}
