#!/bin/bash
make
echo "Running test case[$1] with dead space ratio=$2"
../bin/hw3 ../testcases/$1.hardblocks ../testcases/$1.nets ../testcases/$1.pl ../output/$1.floorplan $2
echo "Done"
