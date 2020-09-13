#!/bin/sh
IN=$1
OUT=`echo $1|sed s/.cpp/.out/g`
echo $IN
echo $OUT
g++ $IN -std=c++11 `pkg-config --cflags --libs opencv4` -o $OUT $2
