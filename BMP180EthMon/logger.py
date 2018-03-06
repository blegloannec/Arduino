#!/usr/bin/env python3

import sys, time
import argparse
from urllib.request import urlopen
import matplotlib.pyplot as plt

# Config.
URL = 'http://192.168.1.20'
LOG = '/home/bastien/.weather/log1.csv'
INTERVAL = 60
RETRY_ITVL = 3

def timestamp():
    return time.strftime('%d-%m-%y %H:%M:%S')

def log_loop(interval):
    log = open(LOG,'a',1)
    while True:
        try:
            U = urlopen(URL)
            T,P = map(float,U.read().decode().split())
            U.close()
            log.write('%s, %f, %.2f, %.1f\n' % (timestamp(),time.time(),T,P))
        except:
            sys.stderr.write('%s: lost (retry in %ds)\n' % (timestamp(),RETRY_ITVL))
            time.sleep(RETRY_ITVL)
            continue
        time.sleep(interval)

def plot_log():
    log = open(LOG,'r')
    L = [l.split(',') for l in log.readlines()]
    log.close()
    X = [float(l[1]) for l in L]
    YT = [float(l[2]) for l in L]
    YP = [float(l[3]) for l in L]
    plt.subplot(2,1,1)
    plt.ylim(10,30)
    plt.plot(X,YT,'-')
    plt.xlabel('t (s)')
    plt.ylabel('T (°C)')
    plt.subplot(2,1,2)
    plt.ylim(950,1050)
    plt.plot(X,YP,'-')
    plt.xlabel('t (s)')
    plt.ylabel('P (hPa)')
    plt.show()

def main():
    argparser = argparse.ArgumentParser()
    argparser.add_argument('--interval','-i',type=int,default=INTERVAL,help='measurements interval in seconds (default: %ds)'%INTERVAL)
    argparser.add_argument('--plot',action='store_true',help='plot current log')
    args = argparser.parse_args()
    if args.plot:
        plot_log()
    else:
        log_loop(args.interval)

main()
