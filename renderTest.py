import pygame
import math
import time
import random

fps = 30
clk = pygame.time.Clock()
BLACK = (0,   0,   0)
WHITE = (255, 255, 255)
BLUE = (0,   0, 255)
GREEN = (0, 255,   0)
RED = (255,   0,   0)


class vec3():
    def __init__(self, x, y, z):
        self.x = float(x)
        self.y = float(y)
        self.z = float(z)

# s is size a is angle
    def project(self, s, a):
        return \
            vec2(s.x / 2 + s.x * self.x / (s.x + 2 * self.z * math.tan(a.x)),
                 s.y / 2 + s.y * self.y / (s.y + 2 * self.z * math.tan(a.y)))

    def __str__(self):
        return "<" + str(self.x) + "," + str(self.y) + "," + str(self.z) + ">"


class vec2():
    def __init__(self, x, y):
        self.x = float(x)
        self.y = float(y)

    def __str__(self):
        return "<" + str(self.x) + "," + str(self.y) + ">"


class vertex():
    def __init__(self, p1, p2):
        self.p1 = p1
        self.p2 = p2


class face():
    def __init__(self, vertexes):
        self.points = vertexes


class cube():
    def __init__(self, pos):
        self.pos = pos

    def getPoints(self):
        return (self.pos, vec3(self.pos.x + 1, self.pos.y, self.pos.z),
                vec3(self.pos.x + 1, self.pos.y + 1, self.pos.z),
                vec3(self.pos.x, self.pos.y + 1, self.pos.z),
                vec3(self.pos.x, self.pos.y + 1, self.pos.z + 1),
                vec3(self.pos.x + 1, self.pos.y, self.pos.z + 1))


size = vec2(640, 512)
angle = vec2(math.radians(30), math.radians(20))

pygame.init()
pygame.display.set_caption("Cubefield Graphics Testing")
screen = pygame.display.set_mode((int(size.x), int(size.y)))
running = True


def drawPixel(p, color=WHITE):
    screen.set_at((int(p.x), int(p.y)), color)


def drawLine(p1, p2, color=WHITE):
    pygame.draw.line(
        screen, color, (int(
            p1.x), int(
            p1.y)), (int(
                p2.x), int(
                    p2.y)), 1)


def drawTri(p1, p2, p3, color=WHITE):
    pass


def drawQuad(p1, p2, p3, p4, color=WHITE):
    pass


numP = 10
points = [[[0 for x in range(numP)]for y in range(numP)]for z in range(numP)]
proj = [[[0 for x in range(numP)]for y in range(numP)]for z in range(numP)]
for x in range(numP):
    for y in range(numP):
        for z in range(numP):
            off = random.random() * 10 - 5
            points[x][y][z] = vec3(
                x * 100 + off - 500,
                y * 100 + off - 500,
                z * 100 + off)

while running:
    screen.fill(BLACK)
    keys = pygame.key.get_pressed()
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    if(keys[pygame.K_q]):
        running = False

    if(keys[pygame.K_a]):
        for i in points:
            for j in i:
                for p in j:
                    p.x += 10

    if(keys[pygame.K_d]):
        for i in points:
            for j in i:
                for p in j:
                    p.x -= 10

    if(keys[pygame.K_w]):
        for i in points:
            for j in i:
                for p in j:
                    p.y += 10

    if(keys[pygame.K_s]):
        for i in points:
            for j in i:
                for p in j:
                    p.y -= 10

    for i in range(numP):
        for j in range(numP):
            for k in range(numP):
                p = points[i][j][k]
                p.z -= 5
                if p.z < -10:
                    p.z = 1000
                projection = p.project(size, angle)
                proj[i][j][k] = projection
                if size.x >= projection.x >= 0 and size.y >= projection.y >= 0:
                    #  drawPixel(projection, 3 * (250 - p.z / 4,))
                    pass
    for i in range(numP - 1):
        for j in range(numP - 1):
            for k in range(numP - 1):
                drawLine(proj[i][j][k], proj[i + 1][j][k])
                drawLine(proj[i][j][k], proj[i][j + 1][k])
                drawLine(proj[i][j][k], proj[i][j][k + 1])
    pygame.transform.flip(screen, False, False)
    pygame.display.flip()
    clk.tick(fps)
