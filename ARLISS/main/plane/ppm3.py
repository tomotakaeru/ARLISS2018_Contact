#!/usr/bin/python
# change to MANUAL for stopping thr

import ppm_class
from time import sleep
from datetime import datetime

ppm = ppm_class.PPM(21)

ppm.update_channels([1500, 1500, 1011, 1500, 1000, 1500, 1500, 1800])
ppm.send()
sleep(5)

ppm.stop()

txt_place="/home/pi/ARLISS/plane/fs_plane.txt"
with open(txt_place, 'a') as f:
    f.write("STOP thr\t" + str(datetime.now()) + "\r\n")
print("STOP thr")
