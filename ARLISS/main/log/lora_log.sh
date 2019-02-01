#!/bin/bash

while :
do
    GPS=$(tail /home/pi/ARLISS/log/gps_log.txt -n 1)
    echo -ne "$GPS\r" > /dev/ttyAMA0
    sleep 2s
done
