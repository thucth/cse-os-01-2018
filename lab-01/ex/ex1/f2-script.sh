#!/bin/bash

for i in 1 2 3 4 5
do 
	echo $i
done

for ((i=1;i<=5;i++))
do
	echo $i
done

echo 'Nhap so n:'
sum=0
read n
echo $n
while [ $n -ge 0 ] # i >= 0
do
	sum=$(($sum+$n))
	n=$(($n-1))
done
echo "Total=$sum"

upper=50
if [ $sum -lt $upper ]
then
	echo "$sum < $upper"
elif [ $sum -eq $upper ]
then
	echo "$sum = $upper"
else
	echo "$sum > $upper"
fi

exit 0
