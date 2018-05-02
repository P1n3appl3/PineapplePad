#ifndef SLIDEPOT_H
#define SLIDEPOT_H

// ADC initialization function, channel 5, PD2
// Input: none
// Output: none
void ADC_Init(void);

//------------ADC_In------------
// Busy-wait Analog to digital conversion, channel 5, PD2
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t ADC_In(void);

#endif
