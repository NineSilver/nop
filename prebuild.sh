#!/usr/bin/sh

if which vc 2&> /dev/null; then
  echo "[nop] vbcc already in path!"
else
  echo "[nop] Adding vbcc to path..."
  
  export VBCC=$(pwd)/tools/vbcc
  export PATH=${VBCC}/bin:${PATH}
fi
