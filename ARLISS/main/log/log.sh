#!/bin/bash

sh /home/pi/ARLISS/log/lora_init.sh

python /home/pi/ARLISS/log/gps_log.py &

sleep 5s

sh /home/pi/ARLISS/log/lora_log.sh &
