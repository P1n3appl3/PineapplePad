import pygame
import math
import time
import random

fps = 20
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


size = v2(160, 128)
angle = v2(math.radians(30), math.radians(20))

pygame.init()
pygame.display.set_caption("Cubefield Graphics Testing")
screen = pygame.display.set_mode((int(size.x), int(size.y)))
running = True


def drawPixel(p, color=WHITE):
    screen.set_at((int(p.x), int(p.y)), color)


class edge():
    def __init__(self, p1, p2):
        self.p1 = p1
        self.p2 = p2

    def draw(self, color=GREEN):
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
        topLeft = edge(
            self.p010.project(
                size, angle), self.p011.project(
                size, angle))
        topRight = edge(
            self.p110.project(
                size, angle), self.p111.project(
                size, angle))
        bottomLeft = edge(
            self.p000.project(
                size, angle), self.p001.project(
                size, angle))
        bottomRight = edge(
            self.p100.project(
                size, angle), self.p101.project(
                size, angle))
        topFront = edge(
            self.p010.project(
                size, angle), self.p110.project(
                size, angle))
        topBack = edge(
            self.p011.project(
                size, angle), self.p111.project(
                size, angle))
        bottomFront = edge(
            self.p000.project(
                size, angle), self.p100.project(
                size, angle))
        bottomBack = edge(
            self.p001.project(
                size, angle), self.p101.project(
                size, angle))
        frontLeft = edge(
            self.p000.project(
                size, angle), self.p010.project(
                size, angle))
        frontRight = edge(
            self.p100.project(
                size, angle), self.p110.project(
                size, angle))
        backLeft = edge(
            self.p001.project(
                size, angle), self.p011.project(
                size, angle))
        backRight = edge(
            self.p101.project(
                size, angle), self.p111.project(
                size, angle))
        topFront.draw()
        #  topBack.draw()
        bottomFront.draw()
        bottomBack.draw()
        bottomRight.draw()
        bottomLeft.draw()
        frontLeft.draw()
        frontRight.draw()
        if self.pos.x > 0:
            backLeft.draw()
            topLeft.draw()
        if self.pos.x + self.size < 0:
            backRight.draw()
            topRight.draw()


def drawTri(p1, p2, p3, color=WHITE):
    pass


def drawQuad(p1, p2, p3, p4, color=WHITE):
    pass


numCubes = 40
cubeSize = 12.5
speed = 6
cubes = []
for i in range(numCubes):
    cubes.append(cube(v3(random.random() * 500 - 250,
                         20, i * 2000 / numCubes + random.random() * 50 - 25),
                      cubeSize))

vel = 0
score = 0
while running:
    screen.fill(BLACK)
    keys = pygame.key.get_pressed()
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    if(keys[pygame.K_q]):
        running = False

    if(keys[pygame.K_a]):
        if vel < 0:
            vel = 0
        vel = min(speed, (vel + .5) * 1.25)
    if(keys[pygame.K_d]):
        if vel > 0:
            vel = 0
        vel = max(-speed, (vel - .5) * 1.25)
    if not (keys[pygame.K_d] or keys[pygame.K_a]):
        vel /= 1.25
        if vel < .1:
            vel = 0

    for c in cubes:
        c.pos.x += vel
        c.pos.z -= speed
        if c.pos.z < -100:
            score += 1
            c.pos.x = random.random() * 500 - 250
            c.pos.z = 500
        elif c.pos.z < -100 + cubeSize and c.pos.x < 0 and c.pos.x + c.size > 0:
            print "You Lose. Score:", score
            running = False
        c.renderWireframe()

    pygame.draw.polygon(screen, WHITE, [[75, 125], [80, 120], [85, 125]])

    pygame.transform.flip(screen, False, False)
    pygame.display.flip()
    clk.tick(fps)
