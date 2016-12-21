#!/bin/bash

for((i=1; i<=3; i++))
do
	echo $i
	root -l -b -q 'MC_weight_check_Tree.C+('$i')'
done

for((i=1; i<=3; i++))
do
	echo $i
	root -l -b -q 'MC_weight_check_Roofit.C+('$i')'
done
