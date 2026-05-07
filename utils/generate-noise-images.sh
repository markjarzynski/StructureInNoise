#!/bin/bash

mkdir -p images/noise

while read -r hash a b;
do 
    ./rng2noise -s 32 -w 1024 -h 1024 -a $a -b $b $hash 
    convert $hash.ppm $hash.png
    mv $hash.png images/noise
done < $1
