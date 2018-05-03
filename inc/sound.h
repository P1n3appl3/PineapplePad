#ifndef _SOUND_H__
#define _SOUND_H__
#include <stdint.h>

void Sound_Init(void);

void Sound_Load();

void Sound_Play();

void Effect_Play(const uint8_t* pt, uint32_t count);

void Sound_Stop(void);

void SD_Mount();

#endif
