#!/usr/bin/python
# -*- coding: utf-8 -*-

import pigpio
from time import sleep
from datetime import datetime
#import sys

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

cut1=0

while cut1==0:
    ld = open("/home/pi/ARLISS/judge_para.txt")
    lines = ld.readlines()
    ld.close()
    for line in lines:
        if line.find("EXPAND_DONE") >= 0:
            cut1=1
    if cut1==1:
        print("cut1_end")
        log_sentence("cut1_end")
        break
    print("cut1")
    log_sentence("cut1")
    cut1_sec(5)
    sleep(1)

pi.stop()
