#!/bin/bash
echo ../verifier/verifier ../testcases/$1.hardblocks ../testcases/$1.nets ../testcases/$1.pl ../output/$1.floorplan $2
../verifier/verifier ../testcases/$1.hardblocks ../testcases/$1.nets ../testcases/$1.pl ../output/$1.floorplan $2