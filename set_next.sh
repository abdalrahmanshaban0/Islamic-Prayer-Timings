#!/usr/bin/bash
FILE="next.txt"
g++ calc_next.cpp -o calc.out
./calc.out > $FILE
rm calc.out

i=0

while read line
do
    PRAY[$i]=$line
    ((i++))
done < $FILE

NAME="${PRAY[0]}"
DIFF="${PRAY[1]}"

#For testing
#DIFF=5
#NAME=0

if [ $NAME -eq 0 ] 
then
    CUR="الفجر"
elif [ $NAME -eq 2 ]
then
    CUR="الظهر"
elif [ $NAME -eq 3 ]
then
    CUR="العصر"
elif [ $NAME -eq 4 ]
then
    CUR="المغرب"
elif [ $NAME -eq 5 ]
then 
    CUR="العشاء"
fi

echo "Comming pray is [$NAME] after $DIFF seconds"

sleep $DIFF;
if [ ! $NAME -eq 1 ]
then
notify-send "مواقيت الصلاة"  "حان الآن موعد آذان $CUR"; 
fi

./set_next.sh
