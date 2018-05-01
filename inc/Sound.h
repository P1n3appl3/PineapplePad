#ifndef __SOUND_H__
#define __SOUND_H__
#include <stdint.h>

void Sound_Init(void);
void Sound_Play(const uint8_t *pt, uint32_t count);
void Sound_Stop(void);

#endif
