#!/bin/bash
#GetYield.C+(iVar, narr, ndphi, version)
#iVar: 0 = y, 1 = Centrality, 2 = pt
#maximum of iarr should be narr-2

version="v8"

root -l -b -q 'GetYield.C+('0', '3', '4', '\"$version\"')'
root -l -b -q 'GetYield.C+('1', '4', '4', '\"$version\"')'
root -l -b -q 'GetYield.C+('2', '4', '4', '\"$version\"')'
