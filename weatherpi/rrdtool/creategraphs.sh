#!/bin/bash

cd rrdtool/

rrdtool graph graph00.png \
    --height=200 --width=900 \
    --color=BACK#FFFFFF \
    --start end-1d --title "24 Hours" \
    DEF:temperature=weatherdata.rrd:temperature:AVERAGE \
	DEF:humidity=weatherdata.rrd:humidity:AVERAGE \
	DEF:altitude=weatherdata.rrd:pressure:AVERAGE \
	DEF:pressure=weatherdata.rrd:pressure:AVERAGE \
    LINE2:temperature#3366CC:"Temperature [°C]" \
	LINE3:humidity#0000ff:"Humidity [%]" \
	LINE4:altitude#ff0000:"Altitude [m]" \
	LINE5:pressure#ff00ff:"Pressure [hPa]"

rrdtool graph graph01.png \
	--height=150 --width=425 \
	--color=BACK#FFFFFF \
	--start end-1d --vertical-label "°C"  --title "Temperature of the last 24 hours" \
	DEF:temperature=weatherdata.rrd:temperature:AVERAGE \
	LINE2:temperature#3366CC

rrdtool graph graph02.png \
    --height=150 --width=425 \
    --color=BACK#FFFFFF \
    --start end-1d --vertical-label "%"  --title "Humidity of the last 24 hours" \
    DEF:humidity=weatherdata.rrd:humidity:AVERAGE \
    LINE2:humidity#3366CC

rrdtool graph graph03.png \
    --height=150 --width=425 \
    --color=BACK#FFFFFF \
    --start end-1d --vertical-label "m"  --title "Altitude of the last 24 hours" \
    DEF:altitude=weatherdata.rrd:altitude:AVERAGE \
    LINE2:altitude#3366CC

rrdtool graph graph04.png \
    --height=150 --width=425 \
    --color=BACK#FFFFFF \
    --start end-1d --vertical-label "hPa"  --title "Pressure of the last 24 hours" \
    DEF:pressure=weatherdata.rrd:pressure:AVERAGE \
    LINE2:pressure#3366CC

cd -

