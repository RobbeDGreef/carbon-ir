if [ ! -d "build" ] 
then
    mkdir build
fi

cd build

cmake .. -DEXEC:BOOL=true
make
mv carbon-ir ../
