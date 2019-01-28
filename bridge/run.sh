#!/usr/bin/env bash

my_dir=`dirname $0`
make && ${my_dir}/bridge
