#!/usr/bin/python
# -*- coding: utf-8 -*-

import pigpio
from time import sleep
from datetime import datetime

pi = pigpio.pi()
txt_place="/home/pi/ARLISS/para.txt"

pi.set_mode(22, pigpio.OUTPUT)
pi.write(22, 0)

def cut1_sec(sec):
    pi.write(22, 1)
    sleep(sec)
    pi.write(22, 0)

def addTime(logtext):
    return str(datetime.now()) + '\t' + logtext + "\r\n"

def log_sentence(value):
    with open(txt_place, 'a') as f:
        f.write(addTime(value))

log_sentence("cut1_initial")
cut1_sec(5)
sleep(1)

pi.stop()
