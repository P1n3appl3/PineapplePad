#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "game.h"

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 128
// precompute tangents of angles (both 30 degrees)
#define VIEW_Y_MULT 5773
#define VIEW_Y_DIV 10000
#define VIEW_X_MULT 5773
#define VIEW_X_DIV 10000
#define SELF_OCCLUSION true

struct Vec2 {
    int x, y;
    Vec2(int x,int  y):x(x), y(y){}
};

struct Vec3 {
    int x, y, z;
    Vec3(int x,int y,int z):x(x), y(y), z(z) {}
    Vec2 project();
};

struct Cube {
    Vec3 pos; // bottom front left from viewport perspective
    int size;
    Cube(Vec3 pos, int size): pos(pos), size(size) {}
    void draw();
}

void drawLine(Vec2 a, Vec2 b);

void drawPlayer();

#endif
