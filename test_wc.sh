#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

echo "Making ..."
make wc &> /dev/null
echo -n "Test 1: "
out="$(./wc wc-in/aaaaaaaaa.in wc-in/aaaaaaaaa.in wc-in/aaaaaaaaa.in wc-in/aaaaaaaaa.in)"
cor="[aaaa] : 130944, [bbbb] : 65472, [a_32736] : 4, [b_16368] : 4,"
if [[ $out =~ "[aaaa] : 130944, [bbbb] : 65472, [a_32736] : 4, [b_16368] : 4," ]]; then
    echo -e "${GREEN}passed"
else
    echo -e "${RED}failed"
    echo -n "Correct output > "
    echo $cor
    echo -n "Your output    > "
    echo $out
fi
