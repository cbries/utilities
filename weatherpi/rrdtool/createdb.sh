#!/bin/bash
rrdtool create weatherdata.rrd --step 60 \
	DS:temperature:GAUGE:120:-50:100 \
	DS:humidity:GAUGE:120:0:100 \
	DS:pressure:GAUGE:120:0:20000 \
	DS:altitude:GAUGE:120:0:1000 \
	RRA:AVERAGE:0.5:1:1440 \
	RRA:MIN:0.5:1:1440 \
	RRA:MAX:0.5:1:1440 \
	RRA:AVERAGE:0.5:30:17520 \
	RRA:MIN:0.5:30:17520 \
	RRA:MAX:0.5:30:17520

