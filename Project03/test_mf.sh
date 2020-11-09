#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

export MAPS="1"
export REDS="1"

echo "MUTUAL FRIENDS TESTS"
echo
echo "CORRECTNESS TESTS"
echo
echo "           file(s) |  size  | mappers | reducers "
echo "-------------------+--------+---------+----------"   
echo "CSCI 1/1:    1     |  small |    1    |     1    "
echo
make mf-capture &> /dev/null

# Test 1: one small file single threaded
echo -n "Test: "
cor="[ Ariel Ryan ] : Eitan,"
out="$(./mf-capture friends/csci.friends)"
if [[ $out =~ "[ Ariel Ryan ] : Eitan," ]]; then
    echo -e "${GREEN}passed${NC}"
else
    echo -e "${RED}failed${NC}"
    echo -n "Correct output > "
    echo $cor
    echo -n "Your output    > "
    echo $out
fi

echo
echo "PERFORMANCE TEST"
echo 
export MAPS="1"
export REDS="1"
echo "Time Mapreduce on many medium Middle-Earth files with 1 mapper(s) 1 reducer(s): "
time ./mf-capture friends/lotr.friends friends/fellowship.friends friends/hobbit.friends friends/lotr.friends friends/fellowship.friends friends/hobbit.friends friends/lotr.friends friends/fellowship.friends friends/hobbit.friends friends/lotr.friends friends/fellowship.friends friends/hobbit.friends friends/lotr.friends friends/fellowship.friends friends/hobbit.friends friends/lotr.friends friends/fellowship.friends friends/hobbit.friends friends/lotr.friends friends/fellowship.friends friends/hobbit.friends friends/lotr.friends friends/fellowship.friends friends/hobbit.friends friends/lotr.friends friends/fellowship.friends friends/hobbit.friends friends/lotr.friends friends/fellowship.friends friends/hobbit.friends friends/lotr.friends friends/fellowship.friends friends/hobbit.friends friends/lotr.friends friends/fellowship.friends friends/hobbit.friends friends/lotr.friends friends/fellowship.friends friends/hobbit.friends &> /dev/null
echo
export MAPS="4"
export REDS="4"
echo "Time Mapreduce on many medium Middle-Earth files with 4 mapper(s) 4 reducer(s): "
time ./mf-capture friends/lotr.friends friends/fellowship.friends friends/hobbit.friends friends/lotr.friends friends/fellowship.friends friends/hobbit.friends friends/lotr.friends friends/fellowship.friends friends/hobbit.friends friends/lotr.friends friends/fellowship.friends friends/hobbit.friends friends/lotr.friends friends/fellowship.friends friends/hobbit.friends friends/lotr.friends friends/fellowship.friends friends/hobbit.friends friends/lotr.friends friends/fellowship.friends friends/hobbit.friends friends/lotr.friends friends/fellowship.friends friends/hobbit.friends friends/lotr.friends friends/fellowship.friends friends/hobbit.friends friends/lotr.friends friends/fellowship.friends friends/hobbit.friends friends/lotr.friends friends/fellowship.friends friends/hobbit.friends friends/lotr.friends friends/fellowship.friends friends/hobbit.friends friends/lotr.friends friends/fellowship.friends friends/hobbit.friends &> /dev/null
echo
make clean &> /dev/null
