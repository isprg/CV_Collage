#!/bin/sh
IN=$1
OUT=`echo $1|sed s/.cpp/.out/g`
echo $IN
echo $OUT
g++ $IN `pkg-config --cflags --libs opencv` -o $OUT $2
