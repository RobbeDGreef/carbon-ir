if [ ! -d "build" ] 
then
    mkdir build
fi

cd build

cmake .. -DEXEC:BOOL=false
make
mv libcarbon-ir.a ../
