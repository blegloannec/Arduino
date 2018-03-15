#!/usr/bin/env python

import sys, random, pygame
from math import sqrt

## INIT
scr_size = scr_w,scr_h = 800,800
FPS = 120
NB_BALLS = 60
EPS = 1e-6
DRAW_SPEED = True
DT = 0.125
BOUNCE = 0.5

def init():
    global scr,clk
    random.seed()
    pygame.init()
    scr = pygame.display.set_mode(scr_size,pygame.DOUBLEBUF)
    pygame.display.set_caption('coconuts')
    clk = pygame.time.Clock()


## COLORS
C_BLACK = (0,0,0)
C_GREY = (192,192,192)

def rand_color():
    return (random.randint(0,255),random.randint(0,255),random.randint(0,255))


## CLASSES
class Ball:
    def __init__(self, px, py, prad):
        self.x = float(px)
        self.y = float(py)
        self.r = prad
        self.col = rand_color()
        self.vx = 0.
        self.vy = 0.
    
    def pos(self):
        return (int(self.x),int(self.y))
    
    def draw(self):
        pygame.draw.circle(scr,self.col,self.pos(),self.r)
        if DRAW_SPEED:
            pygame.draw.line(scr,C_GREY,self.pos(),(int(self.x+self.vx),int(self.y+self.vy)))
        
    def collide(self, B):
        return (self.x-B.x)**2+(self.y-B.y)**2 < (self.r+B.r)**2

    def idist(self, B):
        return self.r+B.r-sqrt((self.x-B.x)**2+(self.y-B.y)**2)


class ConstAccel:
    def get(self):
        return (0.,9.81)


class BallsBox:
    def __init__(self, Accel=None, nb=NB_BALLS):
        self.Accel = ConstAccel() if Accel==None else Accel
        self.nb = nb
        self.balls = []
        while len(self.balls)<self.nb:
            nbrad = random.randint(10,40)
            nbx = random.randint(2*nbrad,scr_h-2*nbrad)
            nby = random.randint(2*nbrad,scr_w-2*nbrad)
            newb = Ball(nbx,nby,nbrad)
            if all(not newb.collide(B) for B in self.balls):
                self.balls.append(newb)

    def draw(self):
        scr.fill(C_BLACK)
        for B in self.balls:
            B.draw()
        pygame.display.flip()

    def collisions(self):
        for ib1 in xrange(self.nb):
            B1 = self.balls[ib1]
            for ib2 in xrange(ib1+1,self.nb):
                B2 = self.balls[ib2]
                if B1.collide(B2):
                    nx,ny = B1.x-B2.x,B1.y-B2.y
                    nn = sqrt(nx*nx+ny*ny)
                    if nn<EPS:
                        continue
                    nx /= nn
                    ny /= nn
                    t1 = B1.vx*nx+B1.vy*ny
                    t2 = B2.vx*nx+B2.vy*ny
                    B1.vx -= (1.+BOUNCE)*t1*nx
                    B1.vy -= (1.+BOUNCE)*t1*ny
                    B2.vx -= (1.+BOUNCE)*t2*nx
                    B2.vy -= (1.+BOUNCE)*t2*ny
                    d = B1.idist(B2)+EPS
                    B1.x += d*nx;
                    B1.y += d*ny;
    
    def step(self, dt=DT):
        ax,ay = self.Accel.get()
        for B in self.balls:
            if B.y+B.r>=scr_h:
                B.y = scr_h-B.r
                B.vy = -BOUNCE*B.vy
            if B.x+B.r>=scr_w:
                B.x = scr_w-B.r
                B.vx = -BOUNCE*B.vx
            if B.x-B.r<=0:
                B.x = B.r
                B.vx = -BOUNCE*B.vx
            if B.y-B.r<=0:
                B.y = B.r
                B.vy = -BOUNCE*B.vy
            B.vx += ax*dt
            B.vy += ay*dt
        self.collisions()
        for B in self.balls:
            B.x += B.vx*dt
            B.y += B.vy*dt


## MAIN
def loop(Accel=None):
    S = BallsBox(Accel)
    while True:
        S.step()
        S.draw()
        for event in pygame.event.get():
            if event.type==pygame.QUIT:
                sys.exit()
            elif event.type==pygame.KEYDOWN and event.key==pygame.K_ESCAPE:
                sys.exit()
            elif event.type==pygame.KEYDOWN and event.key==pygame.K_r:
                S = BallsBox(Accel)
                S.draw()
        clk.tick(FPS)

if __name__=='__main__':
    init()
    loop()
