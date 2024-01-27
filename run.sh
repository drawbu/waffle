#!/bin/sh

[ ! $# -eq 0 ] || exit
target=$1

make $target
[ -e $target ] || exit

xephyr=$(ps -ef | grep Xephyr | grep -v "grep" | wc -l)
unset XDG_SEAT

if [ $xephyr -eq 0 ]
then
    Xephyr -br -ac -reset -screen 1280x720 :1 &
    sleep 1s

    export DISPLAY=:1
fi

./$target "hot-reload"
pkill -f Xephyr
