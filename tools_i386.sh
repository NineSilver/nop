#!/usr/bin/sh

set -xe
read -p "[nop] By pressing Enter, you will be deleting all previous cross-compilers installed. Do you want to proceed?"

rm -rf tools
mkdir -p tools

cd tools
  export PREFIX=$(pwd)
  export TARGET=i386-coff
  export PATH="${PREFIX}/bin:${PATH}"
  
  wget https://ftp.gnu.org/gnu/binutils/binutils-2.21.1.tar.bz2 -O binutils-2.21.1.tar.bz2
  
  tar -xvf binutils-2.21.1.tar.bz2
  mkdir -p binutils-2.21.1/build
  
  cd binutils-2.21.1/build
    ../configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror --enable-obsolete
    
    make -j$(nproc)
    make install
  cd ../..
  
  wget https://ftp.gnu.org/gnu/gcc/gcc-4.4.4/gcc-4.4.4.tar.bz2 -O gcc-4.4.4.tar.bz2
  
  tar -xvf gcc-4.4.4.tar.bz2
  mkdir -p gcc-4.4.4/build
  
  cd gcc-4.4.4/build
    export CC="gcc -fgnu89-inline"
    export CXX="g++ -fgnu89-inline"
    
    ../configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c --without-headers --enable-obsolete
    
    export CC="gcc -fgnu89-inline"
    export CXX="g++ -fgnu89-inline"
    
    make MAKEINFO=true all-gcc -j$(nproc)
    make MAKEINFO=true all-target-libgcc -j$(nproc)
    make MAKEINFO=true install-gcc
    make MAKEINFO=true install-target-libgcc
  cd ../..
cd ..

cd include/nop/arch/i386
  wget http://git.savannah.gnu.org/cgit/grub.git/plain/doc/multiboot2.h?h=multiboot2 -O multiboot2.h
cd ../../..
