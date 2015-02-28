#!/bin/bash
# Web: http://www.makeuseof.com/tag/how-to-capture-time-lapse-photography-with-your-raspberry-pi-and-dslr-or-usb-webcam/
# Timelapse controller for USB webcam
DIR=./timelapsed

# 1440 : 24 hours
#  720 : 12 hours

mkdir -p timelapsed/

x=1
while [ $x -le 1440 ]; do
filename=$(date -u +"%d%m%Y_%H%M-%S").jpg
fswebcam -d /dev/video0 -r 640x480 $DIR/$filename
x=$(( $x + 1 ))
sleep 60;
done;

