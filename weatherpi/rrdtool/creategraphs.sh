#!/bin/bash

cd rrdtool/

rrdtool graph graph00.png \
    --height=200 --width=750 \
    --color=BACK#FFFFFF \
    --start end-1d --vertical-label "?"  --title "24 Hours" \
    DEF:temperature=weatherdata.rrd:temperature:AVERAGE \
	DEF:humidity=weatherdata.rrd:humidity:AVERAGE \
	DEF:altitude=weatherdata.rrd:pressure:AVERAGE \
	DEF:pressure=weatherdata.rrd:pressure:AVERAGE \
    LINE2:temperature#3366CC \
	LINE3:humidity#0000ff \
	LINE4:altitude#ff0000 \
	LINE5:pressure#ff00ff

rrdtool graph graph01.png \
	--height=200 --width=750 \
	--color=BACK#FFFFFF \
	--start end-1d --vertical-label "°C"  --title "Temperatur der letzten 24 Stunden" \
	DEF:temperature=weatherdata.rrd:temperature:AVERAGE \
	LINE2:temperature#3366CC

rrdtool graph graph02.png \
    --height=200 --width=750 \
    --color=BACK#FFFFFF \
    --start end-1d --vertical-label "%"  --title "Humidity der letzten 24 Stunden" \
    DEF:humidity=weatherdata.rrd:humidity:AVERAGE \
    LINE2:humidity#3366CC

rrdtool graph graph03.png \
    --height=200 --width=750 \
    --color=BACK#FFFFFF \
    --start end-1d --vertical-label "m"  --title "Altitude der letzten 60 Minuten" \
    DEF:altitude=weatherdata.rrd:pressure:AVERAGE \
    LINE2:altitude#3366CC

rrdtool graph graph04.png \
    --height=200 --width=750 \
    --color=BACK#FFFFFF \
    --start end-1d --vertical-label "hPa"  --title "Pressure der letzten 60 Minuten" \
    DEF:pressure=weatherdata.rrd:pressure:AVERAGE \
    LINE2:pressure#3366CC

cd -

