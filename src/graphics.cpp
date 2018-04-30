#include "../inc/graphics.h"
#include "../inc/ST7735.h"

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
    drawLine(p000, p100); //front bottom
    drawLine(p010, p110); //front top
    drawLine(p010, p011); //top left
    drawLine(p110, p111); //top right
    drawLine(p000, p010); //front left
    drawLine(p100, p110); //front right
    //depends on camera
    drawLine(p000, p001); //left bottom
    drawLine(p100, p101); //right bottom
    drawLine(p101, p111); //right back
    drawLine(p001, p011); //left back
    drawLine(p001, p101); //bottom back
    drawLine(p011, p111); //top back
}

void drawLine(Vec2 a, Vec2 b) {

}
