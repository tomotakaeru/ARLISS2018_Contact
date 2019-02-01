#!/usr/bin/python
# plane axis judge

import pigpio
from time import sleep
from datetime import datetime

pi = pigpio.pi()

pi.set_mode(18, pigpio.INPUT)
pi.set_pull_up_down(18, pigpio.PUD_UP)
pi.set_mode(23, pigpio.INPUT)
pi.set_pull_up_down(23, pigpio.PUD_UP)

pi.set_mode(19, pigpio.OUTPUT)
pi.write(19, 0)


txt_place="/home/pi/ARLISS/plane/fs_plane.txt"
with open(txt_place, 'a') as f:
    f.write("START\t" + str(datetime.now()) + "\r\n")
print("START")


while (not pi.read(18)) or (not pi.read(23)):
    sleep(0.15)


with open(txt_place, 'a') as f:
    f.write("NON-STOP\t" + str(datetime.now()) + "\r\n")
print("NON-STOP")

# stop ppm1.py
pi.write(19, 1)
sleep(0.2)
pi.write(19, 0)


pi.stop()
