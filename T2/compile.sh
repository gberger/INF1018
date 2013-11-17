#!/bin/sh
gcc -m32 -Wall -Wa,--execstack -o teste teste.c gera.c
