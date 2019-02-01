#!/bin/bash

# put into casing
python /home/pi/ARLISS/plane/ppm1.py &

# plane axis judge
python /home/pi/ARLISS/plane/plane_cable.py

# being stable 5s - ELVup margin 5s
#sleep 1s
# cut2
#sleep 1s

sleep 10s

# ELVup, AUTO
python /home/pi/ARLISS/plane/ppm2.py

# lora
#sh /home/pi/ARLISS/log/log.sh &

sleep 1200s
#sleep 5s

# stop thr
python /home/pi/ARLISS/plane/ppm3.py
