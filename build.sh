#!/bin/bash
set -e
if [[ ! -d "libltdl" ]]; then
    libtoolize
fi
if [[ ! -f "aclocal.m4" ]]; then
    aclocal
fi
if [[ ! -f configure || ./boostrap ]]; then
    autoconf
fi
if [[ ! -f "config.h.in" ]]; then
    autoheader
fi
if [[ ! -f "ltmain.sh" ]]; then
    automake --add-missing
fi
# cp $CONDA_PREFIX/share/gnuconfig/config.* .
./configure --prefix=${CONDA_PREFIX}
