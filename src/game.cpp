#include "../inc/game.h"
#include "../inc/random.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/ST7735.h"
#include "../inc/graphics.h"

#define CUBE_SIZE 15
#define SPREAD 500
#define DIST 500

int vel = 0;
int difficulty = 1;
volatile bool frameDone;

Cube cubes[NUM_CUBES];

int bgColor = BLACK;
int cubeColor = GREEN;
int playerColor = WHITE;

void initField(){
    for (int i = 0; i < NUM_CUBES; ++i) {
        cubes[i] = Cube(Vec3(Random32() % SPREAD - SPREAD / 2,
        -50, 100 + (DIST/NUM_CUBES) * i), CUBE_SIZE);
    }
    shuffleColor();
}

void step(){
    for (int i = 0; i < NUM_CUBES; ++i) {
        cubes[i].pos.z -= difficulty;
        cubes[i].pos.x += vel; // todo: shift and fixed point
        if (cubes[i].pos.z < -50) { // moved behind player
            cubes[i].pos.z = DIST;
            cubes[i].pos.x = Random32() % SPREAD - SPREAD / 2;
        } else if (cubes[i].pos.z < -50 + CUBE_SIZE) { // collision detection
            if (cubes[i].pos.x < 0 && cubes[i].pos.x + cubes[i].size > 0) {
                //todo: display endgame graphic and play endgame sound
                while (true) ;
            }
        }
    }
    frameDone = true;
}

// size being power of 2 allows optimizing mod to bitwise and
static const int palettes[][2] = {
    {BLACK, WHITE},
    {WHITE, BLACK},
    {BLACK, GREEN},
    {WHITE, RED},
    {WHITE, BLUE},
    {BLACK, 1337},
    {WHITE, 1337},
    {GREY, 1337}
};

void shuffleColor(){
    static int palette = 0;
    palette = (palette + 1) % 8;
    bgColor = palettes[palette][0];
    cubeColor = palettes[palette][1];
    for (int i = 0; i < NUM_CUBES; ++i) {
        if (cubeColor == 1337) { // rainbow
            // generate high saturation colors only
            int randColor = Random();
            switch (Random32() % 6) {
            case 0:
                cubes[i].color = ST7735_Color565(0, 255, randColor);
                break;
            case 1:
                cubes[i].color = ST7735_Color565(255, 0, randColor);
                break;
            case 2:
                cubes[i].color = ST7735_Color565(0, randColor, 255);
                break;
            case 3:
                cubes[i].color = ST7735_Color565(255, randColor, 0);
                break;
            case 4:
                cubes[i].color = ST7735_Color565(randColor, 0, 255);
                break;
            case 5:
                cubes[i].color = ST7735_Color565(randColor, 255, 0);
                break;
            }
        }else {
            cubes[i].color = cubeColor;
        }
    }
    playerColor = ~bgColor;
}
