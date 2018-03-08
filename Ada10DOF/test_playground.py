#!/usr/bin/env python

import sys, time, struct, serial
import argparse
import pygame

# Config.
PORT = '/dev/ttyACM0'
BAUD = 115200
dt = 1./60.

# pygame
C_WHITE = (255,255,255)
C_BLACK = (0,0,0)
C_GREY = (50,50,50)
W,H = 800,600
OPT = pygame.DOUBLEBUF


def border(x,vx,xmin,xmax):
    if x<xmin:
        x = xmin
        vx = 0.
    elif x>xmax:
        x = xmax
        vx = 0.
    return x,vx

def init_serial():
    S = serial.Serial(PORT, BAUD, timeout=1)
    if not S.is_open:
        sys.stderr.write('Error: port %s closed\n' % PORT)
        sys.exit(1)
    return S

def init_pygame():
    pygame.display.init()
    Surf = pygame.display.set_mode((W,H),OPT)
    return Surf

def update_surf(Surf,x,y):
    Surf.fill(C_WHITE)
    pygame.draw.circle(Surf,C_GREY,(int(x),int(y)),10)
    pygame.display.flip()

def step_accel(S,x,y,vx,vy):
    S.write(b'a')
    ay,ax,az = struct.unpack('<fff',S.read(12))
    #print('%.3f %.3f %.3f' % (ax,ay,az))
    vx += 150.*ax*dt
    vy += 150.*ay*dt
    x += vx*dt
    y += vy*dt
    return x,y,vx,vy

def step_accel_as_speed(S,x,y,vx,vy):
    S.write(b'a')
    ay,ax,az = struct.unpack('<fff',S.read(12))
    x += 2.*ax
    y += 2.*ay
    return x,y,vx,vy

def step_gyro(S,x,y,vx,vy):
    S.write(b'g')
    vx,vy,vz = struct.unpack('<fff',S.read(12))
    x += 200.*vx*dt
    y += 200.*vy*dt
    return x,y,vx,vy

def step_gyro_as_accel(S,x,y,vx,vy):
    S.write(b'g')
    rx,ry,rz = struct.unpack('<fff',S.read(12))
    vx += rx
    vy -= ry
    x += 40.*vx*dt
    y += 40.*vy*dt
    return x,y,vx,vy

def main():
    argparser = argparse.ArgumentParser()
    argparser.add_argument('--mode',choices=['acc','gyro','acc_as_speed','gyro_as_acc'],help='')
    args = argparser.parse_args()
    step = step_accel
    if args.mode=='gyro':
        step = step_gyro
    elif args.mode=='acc_as_speed':
        step = step_accel_as_speed
    elif args.mode=='gyro_as_speed':
        step = step_gyro_as_accel
    
    S = init_serial()
    Surf = init_pygame()
    
    x,y = W/2.,H/2.
    vx = vy = 0.
    
    while True:
        try:
            x,y,vx,vy = step(S,x,y,vx,vy)
            x,vx = border(x,vx,0,W)
            y,vy = border(y,vy,0,H)
            update_surf(Surf,x,y)
        except:
            pass
        time.sleep(dt)

main()
