#!/bin/sh

set -ex

gcc -Wall -g main.c -o main -I./raylib-5.0_linux_amd64/include/ -L./raylib-5.0_linux_amd64/lib/ -l:libraylib.a -lm

./main
