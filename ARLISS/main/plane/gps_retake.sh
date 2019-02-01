#!/bin/bash

# restart ArduPlane 3.8
sudo systemctl stop apm.service
mv /home/erle/PXFmini/ArduPlane.elf /home/erle/PXFmini/ArduPlane.elf_new
mv /home/erle/PXFmini/ArduPlane.elf_origin /home/erle/PXFmini/ArduPlane.elf
sudo systemctl start apm.service

sleep 15s

# restart ArduPlane 3.10
sudo systemctl stop apm.service
mv /home/erle/PXFmini/ArduPlane.elf /home/erle/PXFmini/ArduPlane.elf_origin
mv /home/erle/PXFmini/ArduPlane.elf_new /home/erle/PXFmini/ArduPlane.elf
sudo systemctl start apm.service
