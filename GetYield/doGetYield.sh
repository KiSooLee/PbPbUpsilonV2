#!/bin/bash
#GetYield.C+(iVar, narr, iarr, ndphi, idphi, version)
#iVar: 0 = y, 1 = Centrality, 2 = pt
#maximum of iarr should be narr-2

version="v4"

root -l -b -q 'GetYield.C+('0', '3', '0', '4', '0', '\"$version\"')'
root -l -b -q 'GetYield.C+('0', '3', '0', '4', '1', '\"$version\"')'
root -l -b -q 'GetYield.C+('0', '3', '0', '4', '2', '\"$version\"')'
root -l -b -q 'GetYield.C+('0', '3', '0', '4', '3', '\"$version\"')'
root -l -b -q 'GetYield.C+('0', '3', '1', '4', '0', '\"$version\"')'
root -l -b -q 'GetYield.C+('0', '3', '1', '4', '1', '\"$version\"')'
root -l -b -q 'GetYield.C+('0', '3', '1', '4', '2', '\"$version\"')'
root -l -b -q 'GetYield.C+('0', '3', '1', '4', '3', '\"$version\"')'
#root -l -b -q 'GetYield.C+('1', '0', '3', '0', '\"$version\"')'
#root -l -b -q 'GetYield.C+('1', '0', '3', '1', '\"$version\"')'
#root -l -b -q 'GetYield.C+('1', '0', '3', '2', '\"$version\"')'
#root -l -b -q 'GetYield.C+('2', '0', '3', '0', '\"$version\"')'
#root -l -b -q 'GetYield.C+('2', '0', '3', '1', '\"$version\"')'
#root -l -b -q 'GetYield.C+('2', '0', '3', '2', '\"$version\"')'
