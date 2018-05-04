// This module contains the ISR that plays a song
// Runs on LM4F120 or TM4C123
// Program written by: Joseph Ryan and Vivek Malneedi
// Last Modified: 3/20/18

#ifndef SONG_H
#define SONG_H

// Initialize SysTick
void Song_Init(void);

void Song_Play(void);

void Song_Stop(void);

#endif
