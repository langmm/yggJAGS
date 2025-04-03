#!/bin/bash
cp $CONDA_PREFIX/share/gnuconfig/config.* .
set -e
./configure --prefix=${CONDA_PREFIX}
