#!/bin/bash

fs_var=$(tail /home/pi/ARLISS/plane/fs_plane.txt -n 1)

echo "REBOOT" >> /home/pi/ARLISS/plane/fs_plane.txt

case "$fs_var" in
MANUAL* | START* ) sh /home/pi/ARLISS/plane/fs_plane_start.sh ;;
NON-STOP* ) python /home/pi/ARLISS/plane/ppm2.py
            sleep 1200s
            python /home/pi/ARLISS/plane/ppm3.py ;;
END* | * ) sh /home/pi/ARLISS/plane/plane.sh ;;
STOP* ) ;;
esac
