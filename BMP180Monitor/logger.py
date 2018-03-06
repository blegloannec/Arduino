#!/usr/bin/env python3

import sys, time, struct, serial
import argparse

# Config.
PORT = '/dev/ttyACM0'
BAUD = 115200
LOGD = '/home/bastien/.weather/'
TMIN,TMAX = 10,30
PMIN,PMAX = 950,1070
INTERVAL = 60
RETRY_ITVL = 3

def perc(x,xmin,xmax):
    p = round(100.*(x-xmin)/(xmax-xmin))
    return max(0,min(100,p))

def timestamp():
    return time.strftime('%d-%m-%y %H:%M:%S')

def main():
    argparser = argparse.ArgumentParser()
    argparser.add_argument('--silent','-s',action='store_true',help='no standard output')
    argparser.add_argument('--interval','-i',type=int,default=INTERVAL,help='measurements interval in seconds (default: %ds)'%INTERVAL)
    args = argparser.parse_args()
    
    S = serial.Serial(PORT, BAUD, timeout=1)
    if not S.is_open:
        sys.stderr.write('Error: port %s closed\n' % PORT)
        sys.exit(1)
    log = open(LOGD+'log.csv','a',1)
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
            if not args.silent:
                print('%s: %.2f hPa, %.1f °C' % (timestamp(),P,T))
            log.write('%s, %.2f, %.1f\n' % (timestamp(),P,T))
            ft.write('%.1f\n' % T)
            ftpc.write('%d\n' % perc(T,TMIN,TMAX))
            fp.write('%.1f\n' % P)
            fppc.write('%d\n' % perc(P,PMIN,PMAX))
        except:
            sys.stderr.write('%s: lost (retry in %ds)\n' % (timestamp(),RETRY_ITVL))
            time.sleep(RETRY_ITVL)
            continue
        time.sleep(args.interval)

main()
