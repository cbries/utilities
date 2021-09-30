#!/bin/bash

#--color=BACK#00000066 \

rrdtool graph graph-12hours.png \
    --height=212 --width=233 \
	--color=BACK#00000066 \
	--color=FONT#ffffff \
	--alt-autoscale-min \
	--alt-autoscale-max \
	--alt-y-grid \
	--border 0 \
	--zoom 1.0 \
	--start end-12h --title "°C | last 12h | Bielefeld, DE" \
	--x-grid MINUTE:30:HOUR:5:HOUR:4:0:%X \
    DEF:temperature=weatherdata.rrd:temperature:AVERAGE \
    LINE2:temperature#3366CC

cp graph-12hours.png ../html/data

