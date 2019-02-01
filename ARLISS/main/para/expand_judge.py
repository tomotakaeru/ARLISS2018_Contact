#!/usr/bin/python
# -*- coding: utf-8 -*-

import pigpio
from time import sleep
from datetime import datetime

pi = pigpio.pi()
txt_place="/home/pi/ARLISS/para.txt"
judge_txt_place="/home/pi/ARLISS/para_judge.txt"

pi.set_mode(5, pigpio.INPUT)
pi.set_pull_up_down(5, pigpio.PUD_UP)
pi.set_mode(19, pigpio.INPUT)
pi.set_pull_up_down(19, pigpio.PUD_UP)

def addTime(logtext):
    return str(datetime.now()) + '\t' + logtext + "\r\n"

def log_sentence(value):
    with open(txt_place, 'a') as f:
        f.write(addTime(value))

while 1:
	print(pi.read(5))
    print(pi.read(19))
    log_sentence()
	if pi.read(5)==1:
        if pi.read(19)==1:
            with open(judge_txt_place, "a") as f:
                f.write("\tEXPAND_DONE\r\n")
            break

pi.stop()
