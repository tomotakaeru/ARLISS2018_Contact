#!/usr/bin/python

import pigpio

pi = pigpio.pi()

pi.set_mode(12, pigpio.OUTPUT)
pi.write(12, 1)

pi.stop()
