import pygame
import math
import time

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
    def __str__(self):
        return "<" + str(self.x) + "," + str(self.y) + "," + str(self.z) + ">"

class vec2():
    def __init__(self, x, y):
        self.x = float(x)
        self.y = float(y)
    def __str__(self):
        return "<" + str(self.x) + "," + str(self.y) + ">"

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
    pygame.draw.line(screen, color, list(p1), list(p2), 4)

def drawTri(p1, p2, p3, color=WHITE):
    pass

def drawQuad(p1, p2, p3, p4, color=WHITE):
    pass

points = []
for x in range(100, 1000, 100):
    for y in range(100, 1000, 100):
        for z in range(100, 1000, 100):
            points.append(vec3(x, y, z))

while running:
    screen.fill(BLACK)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    for p in points:
        projection = vec2(size.x * p.x / (size.x + 2*p.z*math.tan(angle.x)),
                          size.y * p.y / (size.y + 2*p.z*math.tan(angle.y)))
        drawPixel(projection)
    screen = pygame.transform.flip(screen, True, False)
    pygame.display.flip()
    time.sleep(.01666)
    #  clk.tick(fps)
