#!/usr/bin/bash
DIR=~/.config/prayer_timings/
FILE=~/.config/prayer_timings/location.conf

LINK=https://aladhan.com/prayer-times-api#GetTimingsByCity;

if [ ! -f "$FILE" ]; then
    mkdir $DIR
    echo "Enter your country :"
    read CN
    echo "country=$CN" > $FILE
    echo "Enter you City :"
    read CI
    echo "city=$CI" >> $FILE
    echo "Choose a prayer times calculation method (number) from :"
    echo $LINK
    read METHOD
    echo "method=$METHOD" >> $FILE
else
    . $FILE
    CN=$country
    CI=$city
    METHOD=$method
fi

curl -L "http://api.aladhan.com/v1/timingsByCity?city=$CI&country=$CN&method=$METHOD" -H "Accept: application/json" | jq > timings.json

g++ main.cpp -o up.out
./up.out
rm up.out

./set_next.sh

