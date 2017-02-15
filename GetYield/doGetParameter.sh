#!/bin/bash
#GetParameters.C+(iVar, narr, version)
#iVar: 0 = y, 1 = Centrality, 2 = pt
#maximum of iarr should be narr-2

version="v8"

#root -l -b -q 'GetMCParameters.C+('0', '3', '\"$version\"')'
#root -l -b -q 'GetMCParameters.C+('1', '4', '\"$version\"')'
#root -l -b -q 'GetMCParameters.C+('2', '4', '\"$version\"')'

root -l -b -q 'GetParameters.C+('0', '3', '\"$version\"')'
root -l -b -q 'GetParameters.C+('1', '4', '\"$version\"')'
root -l -b -q 'GetParameters.C+('2', '4', '\"$version\"')'
