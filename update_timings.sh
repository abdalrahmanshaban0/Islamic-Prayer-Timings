#!/usr/bin/bash
DIR=home/abdalrahman/.config/prayer_timings/
FILE=/home/abdalrahman/.config/prayer_timings/location.conf

LINK=https://aladhan.com/prayer-times-api#GetTimingsByCity;

if [ ! -f "$FILE" ]; then
    mkdir $DIR
    echo "Enter your country :"
    read CN
    echo "country=$CN" > $FILE
    echo "Enter you City :"
    read CI
    echo "city=$CI" >> $FILE
    echo "Choose a prayer times calculation method (default:5):"
    echo $LINK
    read METHOD
    echo "method=$METHOD" >> $FILE
else
    . $FILE
    CN=$country
    CI=$city
    METHOD=$method
fi

if ping -q -c 1 -W 1 8.8.8.8 > /dev/null
then
curl -L "http://api.aladhan.com/v1/timingsByCity?city=$CI&country=$CN&method=$METHOD" -H "Accept: application/json" | jq > ~/.src/Islamic-Prayer-Timings/timings.json

g++ home/abdalrahman/.src/Islamic-Prayer-Timings/main.cpp -o /home/abdalrahman/.src/Islamic_Prayer_Timings/up.out
/home/abdalrahman/.src/Islamic-Prayer-Timings/up.out
rm /home/abdalrahman/.src/Islamic-Prayer-Timings/up.out
fi
/home/abdalrahman/.src/Islamic-Prayer-Timings/set_next.sh
