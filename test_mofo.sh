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
echo "KERO 2/1:    2     |   med  |    2    |     1    "
echo "KERO 1/2:    2     |   med  |    1    |     2    "
echo "LOTR 2/2:    3     |  large |    2    |     2    "
echo "LOTR 4/4:    3     |  large |    4    |     4    "
echo "LOTR 8/8:    3     |  large |    8    |     8    "
echo
make mf-capture &> /dev/null

# Test 1: one small file single threaded
echo -n "Test 1: "
cor="[ Ariel Ryan ] : Eitan,"
out="$(./wc-capture wc-in/aaaa.in)"
if [[ $out =~ "[ Ariel Ryan ] : Eitan," ]]; then
    echo -e "${GREEN}passed${NC}"
else
    echo -e "${RED}failed${NC}"
    echo -n "Correct output > "
    echo $cor
    echo -n "Your output    > "
    echo $out
fi

# Test 2: multiple small files single threaded
echo -n "Test 2: "
cor="[aaaa] : 6, [bbbb] : 4,"
out="$(./wc-capture wc-in/aaaa.in wc-in/aaaa.in)"
if [[ $out =~ "[aaaa] : 6, [bbbb] : 4," ]]; then
    echo -e "${GREEN}passed${NC}"
else
    echo -e "${RED}failed${NC}"
    echo -n "Correct output > "
    echo $cor
    echo -n "Your output    > "
    echo $out
fi

# Test 3: multiple large files single threaded
echo -n "Test 3: "
cor="[aaaa] : 130944, [bbbb] : 65472, [a_32736] : 4, [b_16368] : 4,"
out="$(./wc-capture wc-in/aaaaaaaaa.in wc-in/aaaaaaaaa.in wc-in/aaaaaaaaa.in wc-in/aaaaaaaaa.in)"
if [[ $out =~ "[aaaa] : 130944, [bbbb] : 65472, [a_32736] : 4, [b_16368] : 4," ]]; then
    echo -e "${GREEN}passed${NC}"
else
    echo -e "${RED}failed${NC}"
    echo -n "Correct output > "
    echo $cor
    echo -n "Your output    > "
    echo $out
fi

export MAPS="2"

# Test 4: multiple large files two threads
echo -n "Test 4: "
cor="[aaaa] : 130944, [bbbb] : 65472, [a_32736] : 4, [b_16368] : 4,"
out="$(./wc-capture wc-in/aaaaaaaaa.in wc-in/aaaaaaaaa.in wc-in/aaaaaaaaa.in wc-in/aaaaaaaaa.in)"
if [[ $out =~ "[aaaa] : 130944, [bbbb] : 65472, [a_32736] : 4, [b_16368] : 4," ]]; then
    echo -e "${GREEN}passed${NC}"
else
    echo -e "${RED}failed${NC}"
    echo -n "Correct output > "
    echo $cor
    echo -n "Your output    > "
    echo $out
fi

export MAPS="1"
export REDS="2"

# Test 5: 
echo -n "Test 5: "
cor="[aaaa] : 130944, [bbbb] : 65472, [a_32736] : 4, [b_16368] : 4,"
out="$(./wc-capture wc-in/aaaaaaaaa.in wc-in/aaaaaaaaa.in wc-in/aaaaaaaaa.in wc-in/aaaaaaaaa.in)"
if [[ $out =~ "[aaaa] : 130944, [bbbb] : 65472, [a_32736] : 4, [b_16368] : 4," ]]; then
    echo -e "${GREEN}passed${NC}"
else
    echo -e "${RED}failed${NC}"
    echo -n "Correct output > "
    echo $cor
    echo -n "Your output    > "
    echo $out
fi

export MAPS="4"
export REDS="4"

# Test 6:
echo -n "Test 6: "
cor="[aaaa] : 327360, [bbbb] : 163680, [a_32736] : 10, [b_16368] : 10,"
out="$(./wc-capture wc-in/aaaaaaaaa.in wc-in/aaaaaaaaa.in wc-in/aaaaaaaaa.in wc-in/aaaaaaaaa.in wc-in/aaaaaaaaa.in wc-in/aaaaaaaaa.in wc-in/aaaaaaaaa.in wc-in/aaaaaaaaa.in wc-in/aaaaaaaaa.in wc-in/aaaaaaaaa.in)"
if [[ $out =~ "[aaaa] : 327360, [bbbb] : 163680, [a_32736] : 10, [b_16368] : 10," ]]; then
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
echo "Time Mapreduce on 8 huge files with 1 mapper 1 reducer: "
time ./wc-capture wc-in/copperfield.in wc-in/copperfield.in wc-in/copperfield.in wc-in/copperfield.in wc-in/copperfield.in wc-in/copperfield.in wc-in/copperfield.in wc-in/copperfield.in &> /dev/null
echo
export MAPS="8"
export REDS="8"
echo "Time Mapreduce on 8 huge files with 8 mapper 8 reducer: "
time ./wc-capture wc-in/copperfield.in wc-in/copperfield.in wc-in/copperfield.in wc-in/copperfield.in wc-in/copperfield.in wc-in/copperfield.in wc-in/copperfield.in wc-in/copperfield.in &> /dev/null
echo
make clean &> /dev/null
