#!/bin/bash

python light.py &

start=0

judge_txt_place="/home/pi/ARLISS/para_judge.txt"

# out of Rocket
while [ $start -eq 0 ]
do
	txt1=`grep -w "START_BY_LUX" $judge_txt_place`
	if [ $? -eq 0 ]
	then
		start=1
		echo start_by_lux
	fi
done

# wait for being stable3
# wait for GPSlock
sleep 30s

python cut1_initial.py

python expand_judge.py &

python cut1.py &

cut1=0

while [ $cut1 -eq 0 ]
do
	txt1=`grep -w "EXPAND_DONE" $judge_txt_place`
	if [ $? -eq 0 ]
	then
		cut1=1
		echo expand_done
	fi
done

# wait for being stable
sleep(5)

#for nose up
sleep(5)

python cut2.py

echo end
