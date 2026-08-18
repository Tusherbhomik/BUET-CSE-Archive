#!/bin/bash

bison -d -y 2005046.y
echo 'Generated parser C file and header'
g++ -w -c -o y.o y.tab.c
echo 'Generated parser object file'
flex 2005046.l
echo 'Generated scanner C file'
g++ -w -c -o l.o lex.yy.c
echo 'Generated scanner object file'
g++ y.o l.o -lfl -o parser
echo 'All ready, running'
./parser input.c
