#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#include "ST7735.h"

#define SCREEN_WIDTH    160
#define SCREEN_HEIGHT   128
// precompute tangents of view angles
#define VIEW_Y_MULT     5773
#define VIEW_Y_DIV      10000
#define VIEW_X_MULT     5773
#define VIEW_X_DIV      10000
#define SELF_OCCLUDE    true

struct Vec2 {
    int x, y;
    Vec2(){
        x = 0; y = 0;
    }

    Vec2(int x, int y) : x(x), y(y){
    }
};

struct Vec3 {
    int x, y, z;
    Vec3(){
        x = 0; y = 0; z = 0;
    }

    Vec3(int x, int y, int z) : x(x), y(y), z(z) {
    }

    Vec2 project();
};

struct Cube {
    Vec3 pos; // bottom front left vertex is origin
    int size;
    int color;
    Cube(){
        size = 0;
    }

    Cube(Vec3 pos, int size, int color = ST7735_WHITE) :
        pos(pos), size(size), color(color) {
    }

    void draw();
};

void drawLine(Vec2 a, Vec2 b, int color = ST7735_WHITE);

void drawPlayer();

void drawGround(int width, int depth);

void printScore(int x);

int randomColor();

#endif
