#!/bin/bash
#GetYield.C+(iVar, iarr, ndphi, idphi, version)
#iVar: 0 = y, 1 = Centrality, 2 = pt

version="v2"

root -l -b -q 'GetYield.C+('0', '0', '3', '0', '\"$version\"')'
root -l -b -q 'GetYield.C+('0', '0', '3', '1', '\"$version\"')'
root -l -b -q 'GetYield.C+('0', '0', '3', '2', '\"$version\"')'
#root -l -b -q 'GetYield.C+('0', '1', '3', '0', '\"$version\"')'
#root -l -b -q 'GetYield.C+('0', '1', '3', '1', '\"$version\"')'
#root -l -b -q 'GetYield.C+('0', '1', '3', '2', '\"$version\"')'
root -l -b -q 'GetYield.C+('1', '0', '3', '0', '\"$version\"')'
root -l -b -q 'GetYield.C+('1', '0', '3', '1', '\"$version\"')'
root -l -b -q 'GetYield.C+('1', '0', '3', '2', '\"$version\"')'
#root -l -b -q 'GetYield.C+('1', '1', '3', '0', '\"$version\"')'
#root -l -b -q 'GetYield.C+('1', '1', '3', '1', '\"$version\"')'
#root -l -b -q 'GetYield.C+('1', '1', '3', '2', '\"$version\"')'
root -l -b -q 'GetYield.C+('2', '0', '3', '0', '\"$version\"')'
root -l -b -q 'GetYield.C+('2', '0', '3', '1', '\"$version\"')'
root -l -b -q 'GetYield.C+('2', '0', '3', '2', '\"$version\"')'
#root -l -b -q 'GetYield.C+('2', '1', '3', '0', '\"$version\"')'
#root -l -b -q 'GetYield.C+('2', '1', '3', '1', '\"$version\"')'
#root -l -b -q 'GetYield.C+('2', '1', '3', '2', '\"$version\"')'
