#include "../inc/graphics.h"
#include "../inc/ST7735.h"
#include "../inc/game.h"

Vec2 Vec3::project() {
    return Vec2(
        SCREEN_WIDTH / 2 + SCREEN_WIDTH * x /
        (SCREEN_WIDTH + 2 * z * VIEW_X_MULT / VIEW_X_DIV),
        SCREEN_HEIGHT / 2 + SCREEN_HEIGHT * y /
        (SCREEN_HEIGHT + 2 * z * VIEW_Y_MULT / VIEW_Y_DIV)
        );
}

void Cube::draw() {
    Vec2 p000 = pos.project();
    Vec2 p001 = Vec3(pos.x, pos.y, pos.z + size).project();
    Vec2 p010 = Vec3(pos.x, pos.y + size, pos.z).project();
    Vec2 p011 = Vec3(pos.x, pos.y + size, pos.z + size).project();
    Vec2 p100 = Vec3(pos.x + size, pos.y, pos.z).project();
    Vec2 p101 = Vec3(pos.x + size, pos.y, pos.z + size).project();
    Vec2 p110 = Vec3(pos.x + size, pos.y + size, pos.z).project();
    Vec2 p111 = Vec3(pos.x + size, pos.y + size, pos.z + size).project();
    //always visible
    drawLine(p000, p100, color); //front bottom
    drawLine(p010, p110, color); //front top
    drawLine(p010, p011, color); //top left
    drawLine(p110, p111, color); //top right
    drawLine(p000, p010, color); //front left
    drawLine(p100, p110, color); //front right
    drawLine(p011, p111, color); //top back
    //depends on camera
    if (SELF_OCCLUDE && pos.x > 0) {
        drawLine(p000, p001, color); //left bottom
        drawLine(p001, p011, color); //left back
    }else if (pos.x + size < 0) {
        drawLine(p101, p111, color); //right back
        drawLine(p100, p101, color); //right bottom
    }
    if (!SELF_OCCLUDE) {
        drawLine(p001, p101, color); //bottom back
    }
}

void drawPlayer(){
    drawLine(Vec2(75, 5), Vec2(80, 10), playerColor);
    drawLine(Vec2(80, 10), Vec2(85, 5), playerColor);
    drawLine(Vec2(85, 5), Vec2(75, 5), playerColor);
}

void swap(int32_t& a, int32_t& b){
    int32_t temp = a;
    a = b;
    b = temp;
}

int abs(int x){
    return x < 0 ? -x : x;
}

void drawLine(Vec2 a, Vec2 b, int color) {
    if (a.x >= SCREEN_WIDTH || a.x < 0 || b.x >= SCREEN_WIDTH || b.x < 0 ||
        a.y >= SCREEN_HEIGHT || a.y < 0 || b.y >= SCREEN_HEIGHT || b.y < 0) {
        return;
    }
    int x0 = a.y, y0 = a.x, x1 = b.y, y1 = b.x; // flipped because landscape
    if (x0 == x1) {
        if (y0 > y1) swap(y0, y1);
        ST7735_DrawFastVLine(x0, y0, y1 - y0 + 1, color);
    } else if (y0 == y1) {
        if (x0 > x1) swap(x0, x1);
        ST7735_DrawFastHLine(x0, y0, x1 - x0 + 1, color);
    } else {
        // Bresenham's algorithm for speedy line drawing
        int16_t steep = abs(y1 - y0) > abs(x1 - x0);
        if (steep) {
            swap(x0, y0);
            swap(x1, y1);
        }
        if (x0 > x1) {
            swap(x0, x1);
            swap(y0, y1);
        }
        int16_t dx, dy;
        dx = x1 - x0;
        dy = abs(y1 - y0);
        int16_t err = dx / 2;
        int16_t ystep;
        if (y0 < y1) {
            ystep = 1;
        } else {
            ystep = -1;
        }
        for (; x0 <= x1; x0++) {
            if (steep) {
                ST7735_DrawPixel(y0, x0, color);
            } else {
                ST7735_DrawPixel(x0, y0, color);
            }
            err -= dy;
            if (err < 0) {
                y0 += ystep;
                err += dx;
            }
        }
    }
}
