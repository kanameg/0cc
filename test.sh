#!/bin/bash

r='\033[0;31m'
g='\033[0;32m'
w='\033[0;39m'

try() {
    expected="$1"
    input="$2"

    ./0cc "${input}" > tmp.s
    gcc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "${actual}" != "${expected}" ]; then
	echo -e "${r}NG:${w} $expected expected, but got $actual"
	exit 1
    else
	echo -e "${g}OK:${w} ${input} = ${actual}"
    fi
}

echo '========== one value'
try 0 '0'
try 128 '128'
try 255 '255'

echo '========== ()'
try 127 '(127)'
try 3 '(1 + 2)'

echo '========== add sub'
try 255 '128+127'
try 0 '128  -	128'
try 60 '10 + 20 + 30'
try 20 '30 - 20 + 10'

echo '========== mul div'
try 200 '10 * 20'
try 24 '2 * 3 * 4'
try 40 '20 * 20 / 10'
try 10 '200 / 20'
try 100 '200 / 20 * 10'

echo '========== mix'
try 26 '2*3+4*5'
try 70 '2*(3+4)*5'

echo '========== variable'
try 25 'a = 12 + 13;'
try 100 'b = (12 + 13) * (10 - 6);'

echo '========== multiple variable'

echo
echo -e "${g}All test is OK${w}"
echo
