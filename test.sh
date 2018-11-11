#!/bin/bash

try() {
    expected="$1"
    input="$2"

    ./0cc "$input" > tmp.s
    gcc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" != "$expected" ]; then
	echo "$expected expected, but got $actual"
	exit 1
    fi
}

echo '========== one value'
try 0 0
try 128 128
try 255 255
try 127 '(127)'

echo '========== add sub'
try 255 '128+127'
try 0 '128  -	128'
try 60 '10 + 20 + 30'

echo '========== mul div'
try 200 '10 * 20'
try 24 '2 * 3 * 4'
try 40 '20 * 20 / 10'
try 10 '200 / 20'

echo '========== mix'
try 26 '2*3+4*5'

echo OK
