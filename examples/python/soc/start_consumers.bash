#!/usr/bin/env bash

num_consumers=$1
counter=1
while [ $counter -le $num_consumers ]
do
	echo $counter
	eval "./video_consumer.py --name=$counter &"
	((counter++))
done
