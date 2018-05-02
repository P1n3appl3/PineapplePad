#include "../inc/game.h"
#include "../inc/random.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/ST7735.h"
#include "../inc/graphics.h"
#include "../inc/sound.h"
#include "../inc/sfx.h"

#define CUBE_SIZE 15
#define SPREAD 500
#define DIST 300
#define GRACE_PERIOD 500

int vel = 0;
int difficulty = 1;
int score = 0;

Cube cubes[NUM_CUBES];

int bgColor = BLACK;
int cubeColor = GREEN;
int playerColor = WHITE;
bool redraw = false;

void initField(){
    for (int i = 0; i < NUM_CUBES; ++i) {
        cubes[i] = Cube(Vec3(Random32() % SPREAD - SPREAD / 2,
                             -50, GRACE_PERIOD + (DIST / NUM_CUBES) * i), CUBE_SIZE);
    }
    shuffleColor();
}

void step(){
    if (redraw) {
        ST7735_FillRect(0, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, bgColor);
        redraw = false;
    }
    for (int i = 0; i < NUM_CUBES; ++i) {
        int tempColor = cubes[i].color;
        cubes[i].color = bgColor;
        cubes[i].draw();
        cubes[i].color = tempColor;
        cubes[i].pos.z -= difficulty;
        cubes[i].pos.x += vel * (1 + difficulty / 2); // todo: shift and fixed point
        if (cubes[i].pos.z < -10) {
            if (cubes[i].pos.z < -20 - CUBE_SIZE) { // moved behind player
                cubes[i].pos.z = DIST;
                cubes[i].pos.x = Random32() % SPREAD - SPREAD / 2;
                if ((score + difficulty) >> 8 > score >> 8) {
                    // next level
                    Effect_Play(beep, sizeof(beep));
                    shuffleColor();
                }
                score += difficulty;
            }
            // collision detection
            else if (cubes[i].pos.x < 0 && cubes[i].pos.x + cubes[i].size > 0) {
                cubes[i].draw();
                //todo: display endgame graphic and play endgame sound
                Effect_Play(dead, sizeof(dead));
                while (true) ;
            }
        }
        cubes[i].draw();
    }
    drawPlayer();
}

// size being power of 2 allows optimizing mod to bitwise and
static const int palettes[][2] = {
    { BLACK, WHITE },
    { WHITE, BLACK },
    { BLACK, GREEN },
    { WHITE, RED },
    { WHITE, BLUE },
    { BLACK, 1337 },
    { WHITE, 1337 },
    { GREEN, BLACK }
}; // todo: fix backwards colors

void shuffleColor(int x){
    static int palette = 0;
    palette = x == -1 ? (palette + 1) % 8 : x;
    bgColor = palettes[palette][0];
    cubeColor = palettes[palette][1];
    for (int i = 0; i < NUM_CUBES; ++i) {
        if (cubeColor == 1337) {    // rainbow
            cubes[i].color = randomColor();
        } else {
            cubes[i].color = cubeColor;
        }
    }
    playerColor = ~bgColor;
    redraw = true;
}
