#!/bin/bash
> out.txt
i=0
while [ $i -lt 1000 ]
do
    #./mf friends/lotr.friends friends/fellowship.friends friends/hobbit.friends  &> /dev/null
    ./mf friends/lotr.friends friends/fellowship.friends &> /dev/null
    #./mf friends/csci.friends friends/csci2.friends friends/csci3.friends  &> /dev/null
    #./mf friends/sgtfrog.friends friends/garurushotai.friends friends/csci3.friends  &> /dev/null
    echo $i
    i=$(($i+1))
done