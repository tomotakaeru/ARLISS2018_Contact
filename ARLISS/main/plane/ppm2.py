#!/usr/bin/python
# ELVup, AUTO

import ppm_class
from time import sleep
from datetime import datetime

ppm = ppm_class.PPM(21)


print("AUTO1") # ELVup
ppm.update_channels([1500, 1900, 1011, 1500, 1000, 1500, 1500, 1200])
ppm.send()
sleep(0.3)

print("ALR&ELVfix")
ppm.update_channels([1500, 1900, 1011, 1500, 1000, 1500, 1500, 1800])
ppm.send()
sleep(32)
#sleep(5)

print("AUTO2 & ELVup")
ppm.update_channels([1500, 1900, 1011, 1500, 1000, 1500, 1500, 1200])
ppm.send()
sleep(7)

print("AUTO3")
ppm.update_channels([1500, 1700, 1011, 1500, 1000, 1500, 1500, 1200])
ppm.send()
sleep(5)


ppm.stop()


txt_place="/home/pi/ARLISS/plane/fs_plane.txt"
with open(txt_place, 'a') as f:
    f.write("END\t" + str(datetime.now()) + "\r\n")
print("END")
