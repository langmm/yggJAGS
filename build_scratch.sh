#!/bin/bash
set -e
if [[ ! -d "libltdl" ]]; then
    libtoolize
fi
aclocal
autoconf -f
autoheader
automake -f
./configure --prefix=${CONDA_PREFIX}
make
