#!/bin/sh

pkill -f Xephyr
Xephyr -br -ac -reset -screen 1280x720 :1 &

sleep 1s
export DISPLAY=:1

./$1
