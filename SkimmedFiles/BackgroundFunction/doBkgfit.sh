#!/bin/bash

#Bkg_fit_function.C+(background function, version)
#Bkg candidate: Exp, Pol1, Cheb1, Cheb2, Erf

Bkg="Exp"
version="v4"

root -l -b -q 'Bkg_fit_function.C+('\"$Bkg\"', '\"$version\"')'
