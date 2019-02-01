#!/usr/bin/python
# -*- coding: utf-8 -*-

import pigpio
from time import sleep
from datetime import datetime

txt_place="/home/pi/ARLISS/para.txt"

pi = pigpio.pi()

pi.set_mode(17, pigpio.OUTPUT)


def cut2_sec(sec):
	pi.write(17, 1)
	sleep(sec)
	pi.write(17, 0)


#for noseup
sleep(5)

print("cut2_start")
cut2_sec(5)

with open(txt_place, 'a') as f:
        f.write(str(datetime.now()) + '\t' + "cut2_end" + "\r\n")

print("cut2_end")
