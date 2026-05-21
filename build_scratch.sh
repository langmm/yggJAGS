#!/bin/bash
set -e
if [[ ! -d "libltdl" ]]; then
    libtoolize
fi
export PKG_CONFIG_PATH=/Users/langmm/yggcpp/_install/lib/pkgconfig
aclocal
autoconf -f
autoheader
automake -f
./configure --prefix=${CONDA_PREFIX} --with-ygginterface
make
make check
# make install
# make installcheck
