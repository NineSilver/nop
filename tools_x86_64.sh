#!/usr/bin/sh

rm -rf tools
mkdir -p tools

cd tools
  wget http://www.ibaug.de/vbcc/vbcc_linux_x64.tar.gz
  
  tar -xvf vbcc_linux_x64.tar.gz
  rm -f vbcc_linux_x64.tar.gz
cd ..
