#!/bin/bash
cd timelapsed/
ls *.jpg > list.txt
filename=$(date -u +"%d%m%Y_%H%M-%S").avi
mencoder -nosound -ovc lavc \
	-lavcopts vcodec=mpeg4:aspect=16/9:vbitrate=8000000 \
	-vf scale=640:480 \
	-o $filename \
	-mf type=jpeg:fps=24 \
	mf://@list.txt
cd -

