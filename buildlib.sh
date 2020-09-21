cd build

cmake .. -DEXEC:BOOL=false
make
mv libcarbon-ir.ar ../