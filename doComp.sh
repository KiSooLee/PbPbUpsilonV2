#!/bin/bash

version1S="v4"
version2S="v4"

root -l -b -q 'CompV2.C+('0', '3', '\"$version1S\"', '\"$version2S\"')'
root -l -b -q 'CompV2.C+('1', '5', '\"$version1S\"', '\"$version2S\"')'
root -l -b -q 'CompV2.C+('2', '5', '\"$version1S\"', '\"$version2S\"')'
