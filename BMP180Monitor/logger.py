#!/usr/bin/env python3

import sys, time, struct, serial

# Config.
PORT = '/dev/ttyACM0'
BAUD = 115200
LOGD = '/home/bastien/.weather/'
TMIN,TMAX = 10,30
PMIN,PMAX = 950,1070

def perc(x,xmin,xmax):
    p = round(100.*(x-xmin)/(xmax-xmin))
    return max(0,min(100,p))

def timestamp():
    return time.strftime('%d-%m-%y %H:%M:%S')

def main():
    S = serial.Serial(PORT, BAUD, timeout=1)
    if not S.is_open:
        sys.stderr.write('Error: port %s closed\n' % PORT)
        sys.exit(1)
    ft = open(LOGD+'conky_temp','w',1)
    ftpc = open(LOGD+'conky_temp_perc','w',1)
    fp = open(LOGD+'conky_press','w',1)
    fppc = open(LOGD+'conky_press_perc','w',1)
    while True:
        try:
            S.write(b'q')
            #S.flush()
            P,T = struct.unpack('<ff',S.read(8))
            P /= 100
            print('%s: %.2f hPa, %.1f °C' % (timestamp(),P,T))
            ft.write('%.1f\n' % T)
            ftpc.write('%d\n' % perc(T,TMIN,TMAX))
            fp.write('%.1f\n' % P)
            fppc.write('%d\n' % perc(P,PMIN,PMAX))
        except:
            sys.stderr.write('lost\n')
            pass
        time.sleep(1)

main()
