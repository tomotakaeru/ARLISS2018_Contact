#!/usr/bin/python
# put into casing 

import ppm_class
import pigpio
from time import sleep
from datetime import datetime

ppm = ppm_class.PPM(21)
pi19 = pigpio.pi()

txt_place="/home/pi/ARLISS/plane/fs_plane.txt"
with open(txt_place, 'a') as f:
    f.write("MANUAL\t" + str(datetime.now()) + "\r\n")
print("MANUAL")

# [ALR, ELV, THR, x, FLAP, x, x, x]
#ppm.update_channels([1500, 1500, 1011, 1500, 1000, 1500, 1500, 1800])
ppm.update_channels([1500, 1500, 1011, 1500, 2000, 1500, 1500, 1800])
ppm.send()
sleep(5)

while pi19.read(19)==0:
    sleep(0.15)

pi19.stop()
ppm.stop()
