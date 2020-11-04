#!/bin/bash
to=$1
from=$2
config=$3
keyword=$4
password=''
while read -r line; do
    password+=${line}
done < config
./smtp $to@gmail.com $from@gmail.com $password $keyword