#!/bin/bash
ARGC=$#
ARGV0=$0
ARGV1=$1

SOURCE="${BASH_SOURCE[0]}"
DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

cd "$DIR"

SOURCE_DIRNAME=.
TARGET_DIRNAME=.

if [ $ARGC -lt 1 ];
then
	SOURCE_DIRNAME=Input
	TARGET_DIRNAME=Output
else
	SOURCE_DIRNAME=$ARGV1
	TARGET_DIRNAME=$SOURCE_DIRNAME/../Output
fi

IFS=$'\n'

rm -rf $TARGET_DIRNAME/*
if [ ! -d "$TARGET_DIRNAME" ]; then
	mkdir $TARGET_DIRNAME
fi
	
echo "We are in: "`pwd`
echo "We will use: $SOURCE_DIRNAME"
	
for i in $( ls $SOURCE_DIRNAME/*.mp3 ); do	
	TARGET_FILENAME=`basename $i .mp3`
	TARGET_FILENAME_FULL=$TARGET_DIRNAME/$TARGET_FILENAME.mp3		
	./ffmpeg.exe -i $i -acodec libmp3lame -aq 2 -ss 00:00:02 $TARGET_FILENAME_FULL	
done
