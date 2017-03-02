#!/bin/bash
#GetMCParameters.C+(iVar, narr, iS, version)
#GetParameters.C+(iVar, narr, version)
#iVar: 0 = y, 1 = Centrality, 2 = pt
#narr = nbin+1

version="v6"

#root -l -b -q 'GetMCParameters.C+('2', '2', '1', '\"$version\"')'
#root -l -b -q 'GetMCParameters.C+('0', '3', '1', '\"$version\"')'
#root -l -b -q 'GetMCParameters.C+('1', '4', '1', '\"$version\"')'
#root -l -b -q 'GetMCParameters.C+('2', '4', '1', '\"$version\"')'

root -l -b -q 'GetParameters.C+('2', '2', '\"$version\"')'
#root -l -b -q 'GetParameters.C+('0', '3', '\"$version\"')'
#root -l -b -q 'GetParameters.C+('1', '4', '\"$version\"')'
#root -l -b -q 'GetParameters.C+('2', '4', '\"$version\"')'
