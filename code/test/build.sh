#!/bin/sh

cd /home/nachos/NachOS-4.0/code/build.linux/
make
cd /home/nachos/NachOS-4.0/code/test 
make $1