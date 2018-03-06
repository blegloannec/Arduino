#!/bin/bash

IP=192.168.1.20
DIR=~/.weather
TMIN=10
TMAX=30
PMIN=950
PMAX=1070

#TP=$(wget -q -O- http://$IP | tr -d '\n\r')
TP=$(curl -s http://$IP | tr -d '\n\r')

T=$(echo $TP | cut -d' ' -f1)
P=$(echo $TP | cut -d' ' -f2)
Tpc=$(echo "100*($T-$TMIN)/($TMAX-$TMIN)" | bc -l)
Ppc=$(echo "100*($P-$PMIN)/($PMAX-$PMIN)" | bc -l)

echo $T > $DIR/conky_temp
echo $P > $DIR/conky_press
echo $Tpc > $DIR/conky_temp_perc
echo $Ppc > $DIR/conky_press_perc
