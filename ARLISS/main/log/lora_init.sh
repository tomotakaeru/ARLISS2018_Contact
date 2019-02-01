#!/bin/bash

# lora ON
python /home/pi/ARLISS/log/lora_on.py

# config for ttyAMA0
stty -F /dev/ttyAMA0 speed 115200 -parenb -parodd -cmspar cs8 hupcl -cstopb cread clocal -crtscts -ignbrk -brkint -ignpar -parmrk -inpck -istrip -inlcr igncr -icrnl -ixon -ixoff -iuclc -ixany -imaxbel -iutf8 -opost -olcuc -ocrnl -onlcr -onocr -onlret -ofill -ofdel nl0 cr0 tab0 bs0 vt0 ff0 isig icanon iexten -echo -echoe -echok -echonl -noflsh -xcase -tostop -echoprt -echoctl -echoke
cat /dev/ttyAMA0 &


# @config mode ownid,dstid
#echo -ne "1\r" > /dev/ttyAMA0

# goto operation mode
#echo -ne "start\r" > /dev/ttyAMA0
