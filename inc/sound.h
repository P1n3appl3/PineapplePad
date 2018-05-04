#ifndef _SOUND_H__
#define _SOUND_H__
#include <stdint.h>

void Sound_Init(void);

void Sound_Play(uint32_t period);

void Sound_Stop(void);

void Effect_Play(const uint8_t* pt, uint32_t count);

#endif
