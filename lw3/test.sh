#!/bin/bash
files=(example.html picture.png schema.json style.css text.txt)
for ((i=1; i < 50; i++))
do
let "idx=$((((RANDOM<<15)|RANDOM) % 5))"
let "port=$((((RANDOM<<15)|RANDOM) % 10 + 1024))"
./client.out $port data/${files[idx]} > /dev/null
done
