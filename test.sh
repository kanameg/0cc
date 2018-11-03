#!/bin/bash

try() {
    expected="$1"
    input="$2"

    ./9cc "$input" > tmp.s
    gcc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" != "$expected" ]; then
	echo "$input expected, but got $actual"
	exit 1
    fi
}

try 0 0
try 128 128
try 255 255

try 255 '128+127'
try 0 '128-128'
try 255 '128-128+255'

try 255 '128 + 127'
try 0 '128 - 128'
try 255 '128	- 128 +	255'

try 255 '  128	- 128 +	255   '

echo OK
