#!/usr/bin/env bash

my_dir=`dirname $0`

# Build and run.
make 2>&1 |tee make.out && ${my_dir}/a.out |tee output.txt
