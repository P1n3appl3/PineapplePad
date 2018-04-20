import pygame
import math
import time
import random

print "yo"

fps = 30
clk = pygame.time.Clock()
BLACK = (0,   0,   0)
WHITE = (255, 255, 255)
BLUE = (0,   0, 255)
GREEN = (0, 255,   0)
RED = (255,   0,   0)


class v2():
    def __init__(self, x, y):
        self.x = float(x)
        self.y = float(y)

    def __str__(self):
        return "<" + str(self.x) + "," + str(self.y) + ">"


class v3():
    def __init__(self, x, y, z):
        self.x = float(x)
        self.y = float(y)
        self.z = float(z)

# s is size of viewing plane a is angle of fov
    def project(self, s, a):
        return \
            v2(s.x / 2 + s.x * self.x / (s.x + 2 * self.z * math.tan(a.x)),
               s.y / 2 + s.y * self.y / (s.y + 2 * self.z * math.tan(a.y)))

    def __str__(self):
        return "<" + str(self.x) + "," + str(self.y) + "," + str(self.z) + ">"


size = v2(640, 512)
#  size = v2(160, 128)
angle = v2(math.radians(30), math.radians(20))

pygame.init()
pygame.display.set_caption("Cubefield Graphics Testing")
screen = pygame.display.set_mode((int(size.x), int(size.y)))
running = True


def drawPixel(p, color=WHITE):
    screen.set_at((int(p.x), int(p.y)), color)


class vertex():
    def __init__(self, p1, p2):
        self.p1 = p1
        self.p2 = p2

    def draw(self, color=WHITE):
        pygame.draw.line(
            screen, color, (int(
                self.p1.x), int(
                self.p1.y)), (int(
                    self.p2.x), int(
                    self.p2.y)), 1)


class cube():
    def __init__(self, pos, size):
        self.pos = pos
        self.size = size
        self.computePoints()

    def computePoints(self):
        self.p000 = self.pos
        self.p001 = v3(
            self.pos.x,
            self.pos.y,
            self.pos.z +
            self.size)
        self.p010 = v3(
            self.pos.x,
            self.pos.y +
            self.size,
            self.pos.z)
        self.p011 = v3(
            self.pos.x,
            self.pos.y +
            self.size,
            self.pos.z +
            self.size)
        self.p100 = v3(
            self.pos.x +
            self.size,
            self.pos.y,
            self.pos.z)
        self.p101 = v3(
            self.pos.x +
            self.size,
            self.pos.y,
            self.pos.z +
            self.size)
        self.p110 = v3(
            self.pos.x +
            self.size,
            self.pos.y +
            self.size,
            self.pos.z)
        self.p111 = v3(
            self.pos.x +
            self.size,
            self.pos.y +
            self.size,
            self.pos.z +
            self.size)

    def renderWireframe(self):
        self.computePoints()
        topLeft = vertex(
            self.p010.project(
                size, angle), self.p011.project(
                size, angle))
        topRight = vertex(
            self.p110.project(
                size, angle), self.p111.project(
                size, angle))
        bottomLeft = vertex(
            self.p000.project(
                size, angle), self.p001.project(
                size, angle))
        bottomRight = vertex(
            self.p100.project(
                size, angle), self.p101.project(
                size, angle))
        topFront = vertex(
            self.p010.project(
                size, angle), self.p110.project(
                size, angle))
        topBack = vertex(
            self.p011.project(
                size, angle), self.p111.project(
                size, angle))
        bottomFront = vertex(
            self.p000.project(
                size, angle), self.p100.project(
                size, angle))
        bottomBack = vertex(
            self.p001.project(
                size, angle), self.p101.project(
                size, angle))
        frontLeft = vertex(
            self.p000.project(
                size, angle), self.p010.project(
                size, angle))
        frontRight = vertex(
            self.p100.project(
                size, angle), self.p110.project(
                size, angle))
        backLeft = vertex(
            self.p001.project(
                size, angle), self.p011.project(
                size, angle))
        backRight = vertex(
            self.p101.project(
                size, angle), self.p111.project(
                size, angle))
        topLeft.draw()
        topRight.draw()
        bottomLeft.draw()
        bottomRight.draw()
        topFront.draw()
        topBack.draw()
        bottomFront.draw()
        bottomBack.draw()
        frontLeft.draw()
        frontRight.draw()
        backLeft.draw()
        backRight.draw()


def drawTri(p1, p2, p3, color=WHITE):
    pass


def drawQuad(p1, p2, p3, p4, color=WHITE):
    pass


numCubes = 20
cubeSize = 50
speed = 5
cubes = []
for i in range(numCubes):
    cubes.append(cube(v3(random.random() * 800 - 400,
                         100, i * 1000 / numCubes), cubeSize))

while running:
    screen.fill(BLACK)
    keys = pygame.key.get_pressed()
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    if(keys[pygame.K_q]):
        running = False

    if(keys[pygame.K_a]):
        for c in cubes:
            c.pos.x += speed

    if(keys[pygame.K_d]):
        for c in cubes:
            c.pos.x -= speed

    if(keys[pygame.K_s]):
        for c in cubes:
            c.pos.y -= speed

    if(keys[pygame.K_w]):
        for c in cubes:
            c.pos.y += speed

    for c in cubes:
        c.pos.z -= speed
        if c.pos.z < -30:
            c.pos.z = 1000
        c.renderWireframe()

    pygame.transform.flip(screen, False, False)
    pygame.display.flip()
    clk.tick(fps)
