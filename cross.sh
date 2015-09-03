#!/bin/bash

if [[ "$1" == "" ]]
then
   echo "Usage: $0 /path/to/install/dir"
   exit
fi

echo "Ensure you have libgmp3-dev libmpfr-dev libisl-dev libcloog-isl-dev libmpc-dev texinfo installed, as well as build-essential"

export PREFIX="$1"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

mkdir -p $PREFIX/build/tarballs
cd $PREFIX/build/tarballs

wget "ftp://ftp.gnu.org/gnu/gcc/gcc-4.8.4/gcc-4.8.4.tar.gz"
wget "ftp://ftp.gnu.org/gnu/binutils/binutils-2.25.tar.gz"

mkdir -p $PREFIX/build
cd $PREFIX/build

tar -zxf "tarballs/binutils-2.25.tar.gz"
tar -zxf "tarballs/gcc-4.8.4.tar.gz"

mkdir $PREFIX/build/binutils-build
cd $PREFIX/build/binutils-build

../binutils-2.25/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install

mkdir -p $PREFIX/build/gcc
cd $PREFIX/build/gcc

which -- $TARGET-as || echo "$TARGET-as is not in the PATH"

../gcc-4.8.4/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers

make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
