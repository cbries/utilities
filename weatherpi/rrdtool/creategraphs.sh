#!/bin/bash

cd rrdtool/

rrdtool graph graph00.png \
    --height=200 --width=900 \
    --color=BACK#FFFFFF \
    --start end-1d --title "24 Hours" \
	--x-grid MINUTE:10:HOUR:2:HOUR:2:0:%X \
    --alt-autoscale --alt-y-grid --rigid \
	DEF:temperature=weatherdata.rrd:temperature:AVERAGE \
	DEF:humidity=weatherdata.rrd:humidity:AVERAGE \
	LINE3:temperature#3366CC:"Temperature [°C]" \
	LINE3:humidity#ff00AA:"Humidity [%]" 
	
	#LINE2:altitude2#ff0000:"Altitude [m]" 
	#LINE:pressure#ff00ff:"Pressure [hPa]"
    #DEF:altitude=weatherdata.rrd:pressure:AVERAGE \
    #DEF:pressure=weatherdata.rrd:pressure:AVERAGE \

rrdtool graph graph01.png \
	--height=150 --width=425 \
	--color=BACK#FFFFFF \
	--start end-1d --vertical-label "°C"  --title "Temperature of the last 24 hours" \
	--x-grid MINUTE:30:HOUR:5:HOUR:5:0:%X \
	--alt-autoscale --alt-y-grid --rigid \
	DEF:temperature=weatherdata.rrd:temperature:AVERAGE \
	LINE2:temperature#3366CC

rrdtool graph graph02.png \
    --height=150 --width=425 \
    --color=BACK#FFFFFF \
    --start end-1d --vertical-label "%"  --title "Humidity of the last 24 hours" \
	--x-grid MINUTE:30:HOUR:5:HOUR:5:0:%X \
	--alt-autoscale --alt-y-grid --rigid \
    DEF:humidity=weatherdata.rrd:humidity:AVERAGE \
    LINE2:humidity#3366CC

rrdtool graph graph03.png \
    --height=150 --width=425 \
    --color=BACK#FFFFFF \
    --start end-1d --vertical-label "m"  --title "Altitude of the last 24 hours" \
	--x-grid MINUTE:30:HOUR:5:HOUR:5:0:%X \
	--alt-autoscale --alt-y-grid --rigid \
    DEF:altitude=weatherdata.rrd:altitude:AVERAGE \
    LINE2:altitude#3366CC

rrdtool graph graph04.png \
    --height=150 --width=425 \
    --color=BACK#FFFFFF \
    --start end-1d --vertical-label "hPa"  --title "Pressure of the last 24 hours" \
	--x-grid MINUTE:30:HOUR:5:HOUR:5:0:%X \
	--alt-autoscale --alt-y-grid --rigid \
    DEF:pressure=weatherdata.rrd:pressure:AVERAGE \
    LINE2:pressure#3366CC

cd -

