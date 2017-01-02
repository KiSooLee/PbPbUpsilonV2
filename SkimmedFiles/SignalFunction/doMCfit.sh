#!/bin/bash

#MC_fit_function.C+(state, signal function, background function, version)
#Sig candidate: Gaus, twoGaus, CBGaus, CB, twoCB
#Bkg candidate: Expo, Pol1, Cheb1, Cheb2, Erf

Sig="Gaus"
Bkg="Pol1"
version="v4"

for((i=1; i<=3; i++))
do
	echo $i
	root -l -b -q 'MC_fit_function.C+('$i', '\"$Sig\"', '\"$Bkg\"', '\"$version\"')'
done
