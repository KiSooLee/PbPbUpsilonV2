#!/bin/bash
#GetMCParameters.C+(iVar, narr, iS, version)
#GetParameters.C+(iVar, narr, version)
#GetYield.C+(iVar, narr, ndphi, version)
#iVar: 0 = y, 1 = Centrality, 2 = pt
#narr = nbin+1
#maximum of iarr should be narr-2

version="v9"

#root -l -b -q 'GetMCParameters.C+('1', '2', '1', '\"$version\"')'
#root -l -b -q 'GetMCParameters.C+('0', '3', '1', '\"$version\"')'
#root -l -b -q 'GetMCParameters.C+('1', '3', '1', '\"$version\"')'
#root -l -b -q 'GetMCParameters.C+('2', '3', '1', '\"$version\"')'

#root -l -b -q 'GetParameters.C+('1', '2', '\"$version\"')'
#root -l -b -q 'GetParameters.C+('0', '3', '\"$version\"')'
#root -l -b -q 'GetParameters.C+('1', '3', '\"$version\"')'
#root -l -b -q 'GetParameters.C+('2', '3', '\"$version\"')'

#root -l -b -q 'GetYield.C+('1', '2', '4', '\"$version\"')'
root -l -b -q 'GetYield.C+('0', '3', '4', '\"$version\"')'
root -l -b -q 'GetYield.C+('1', '3', '4', '\"$version\"')'
root -l -b -q 'GetYield.C+('2', '3', '4', '\"$version\"')'
