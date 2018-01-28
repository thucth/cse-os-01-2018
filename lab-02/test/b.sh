#!/bin/bash

narg=$#;

a=$1;
b=$2;

echo $@;
echo $args;
echo $((a+b));

if [ $a -ge $b ]
then
	echo "$a greater or equal than $b"
else
	echo "$a less than $b"
fi
