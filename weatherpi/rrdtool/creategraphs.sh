#!/bin/bash

cd rrdtool/

rrdtool graph graph00.png \
    --height=200 --width=900 \
    --color=BACK#FFFFFF \
    --start end-1d --title "24 Hours - Temperature [°C], Humidity [%]" \
	--x-grid MINUTE:10:HOUR:2:HOUR:2:0:%X \
    --alt-autoscale --alt-y-grid --rigid \
	DEF:temperature=weatherdata.rrd:temperature:AVERAGE \
	DEF:humidity=weatherdata.rrd:humidity:AVERAGE \
	LINE3:temperature#3366CC \
	LINE3:humidity#ff00AA 
cp graph00.png ../html/data	

rrdtool graph graph00-portrait.png \
    --height=400 --width=900 \
    --color=BACK#FFFFFF \
    --start end-1d --title "24 Hours - Temperature [°C], Humidity [%]" \
    --x-grid MINUTE:10:HOUR:2:HOUR:2:0:%X \
    --alt-autoscale --alt-y-grid --rigid \
    DEF:temperature=weatherdata.rrd:temperature:AVERAGE \
    DEF:humidity=weatherdata.rrd:humidity:AVERAGE \
    LINE3:temperature#3366CC \
    LINE3:humidity#ff00AA
cp graph00-portrait.png ../html/data

	#LINE2:altitude2#ff0000:"Altitude [m]" 
	#LINE:pressure#ff00ff:"Pressure [hPa]"
    #DEF:altitude=weatherdata.rrd:pressure:AVERAGE \
    #DEF:pressure=weatherdata.rrd:pressure:AVERAGE \

rrdtool graph graph01.png \
	--height=150 --width=425 \
	--color=BACK#FFFFFF \
	--start end-1d --title "Temperature [°C] of the last 24 hours" \
	--x-grid MINUTE:30:HOUR:5:HOUR:5:0:%X \
	--alt-autoscale --alt-y-grid --rigid \
	DEF:temperature=weatherdata.rrd:temperature:AVERAGE \
	LINE2:temperature#3366CC
cp graph01.png ../html/data

rrdtool graph graph02.png \
    --height=150 --width=425 \
    --color=BACK#FFFFFF \
    --start end-1d --title "Humidity [%] of the last 24 hours" \
	--x-grid MINUTE:30:HOUR:5:HOUR:5:0:%X \
	--alt-autoscale --alt-y-grid --rigid \
    DEF:humidity=weatherdata.rrd:humidity:AVERAGE \
    LINE2:humidity#3366CC
cp graph02.png ../html/data

rrdtool graph graph03.png \
    --height=150 --width=425 \
    --color=BACK#FFFFFF \
    --start end-1d --title "Altitude [m] of the last 24 hours" \
	--x-grid MINUTE:30:HOUR:5:HOUR:5:0:%X \
	--alt-autoscale --alt-y-grid --rigid \
    DEF:altitude=weatherdata.rrd:altitude:AVERAGE \
    LINE2:altitude#3366CC
cp graph03.png ../html/data

rrdtool graph graph04.png \
    --height=150 --width=425 \
    --color=BACK#FFFFFF \
    --start end-1d --title "Pressure [hPa] of the last 24 hours" \
	--x-grid MINUTE:30:HOUR:5:HOUR:5:0:%X \
	--alt-autoscale --alt-y-grid --rigid \
    DEF:pressure=weatherdata.rrd:pressure:AVERAGE \
    LINE2:pressure#3366CC
cp graph04.png ../html/data

cd -

