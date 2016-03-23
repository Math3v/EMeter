#!/bin/bash

#echo "set terminal png large size 4096,4096
echo "set terminal svg
set output 'plot.svg' 
plot 'data.txt' using 1:2 lc rgb 'red' pt 21" | gnuplot