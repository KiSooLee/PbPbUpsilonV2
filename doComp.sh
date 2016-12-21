#!/bin/bash

for((i=0; i<3; i++))
do
	root -l -b -q 'CompV2.C+('$i')'
done
