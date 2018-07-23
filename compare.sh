#!/bin/bash

meanA=$(awk '{/CPUID:/ counter++; total+=$2}; END {printf("%d\n", total/counter)}' $1)
meanB=$(awk '{/CPUID:/ counter++; total+=$2}; END {printf("%d\n", total/counter)}' $2)

echo "Mean seq0 = " $meanA
echo "Mean seq1 = " $meanB

awk -v mean_A=$meanA -v mean_B=$meanB '{}; END { printf("%.2f %\n", ((mean_B-mean_A)/mean_A) * 100)}' /dev/null
