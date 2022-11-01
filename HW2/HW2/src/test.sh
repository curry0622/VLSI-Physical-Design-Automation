#!/bin/bash
make
../bin/hw2 test.cells test.nets out
cd ../verifier
./verify ../src/test.cells ../src/test.nets ../src/out