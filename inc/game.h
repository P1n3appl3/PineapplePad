#ifndef _GAME_H
#define _GAME_H
#include "graphics.h"

#define NUM_CUBES 20

extern int vel;
extern int difficulty;
extern volatile bool frameDone;
extern Cube cubes[];

extern int bgColor;
extern int cubeColor;
extern int playerColor;

void initField();

void step();

void shuffleColor();

#endif
