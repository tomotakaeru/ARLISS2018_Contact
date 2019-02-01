#!/usr/bin/python

import pigpio
from time import sleep

pi = pigpio.pi()

pi.set_mode(19, pigpio.OUTPUT)
pi.write(19, 1)
print("19pin ON")
sleep(0.2)
pi.write(19, 0)
print("19pin OFF")

pi.stop()
