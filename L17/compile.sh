#!/bin/sh
gcc -m32 -c -Wall temp1.c
gcc -m32 -c -Wall temp2.c
gcc -m32 -o prog  temp1.o temp2.o
