#!/bin/bash

FILE="/home/abdalrahman/.src/Islamic-Prayer-Timings/next.txt"

while true
do
    /home/abdalrahman/.src/Islamic-Prayer-Timings/calc_next.o > $FILE
    sleep 1
done
