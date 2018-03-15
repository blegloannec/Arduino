#!/usr/bin/env python

import sys, time, struct, serial
import cocolib

# Config.
PORT = '/dev/ttyACM0'
BAUD = 115200

def init_serial():
    S = serial.Serial(PORT, BAUD, timeout=1)
    if not S.is_open:
        sys.stderr.write('Error: port %s closed\n' % PORT)
        sys.exit(1)
    return S


class Accelerometer:
    def __init__(self):
        self.S = init_serial()

    def get(self):
        while True:
            try:
                self.S.write(b'a')
                ay,ax,az = struct.unpack('<fff',self.S.read(12))
                break
            except:
                pass
        return ax,ay


def main():
    cocolib.init()
    A = Accelerometer()
    cocolib.loop(A)

main()
