#!/bin/bash
set -exo pipefail
export MAKEFLAGS="-j2"
apt-get update
apt-get -y install libgmp-dev libmpfr-dev libflint-dev valgrind libflint-arb-dev libboost-all-dev libtool make autoconf $PACKAGES
./bootstrap.sh
./configure --prefix=/usr --enable-valgrind ${CONFIGURE} || (cat config.log; false)
make
ldd .libs/libeantic.so
make check || (cat `find test-suite.log` /dev/null; false)
make check-valgrind || (cat `find test-suite-memcheck.log` /dev/null; false)
make distcheck
