# Carbon Intermediate Representation Language
The carbon intermediate representation language or carbon for short
is a architecture independent assembly-like language built to
easily connect a front end compiler to multiple backend architectures.

## Build
Before you build carbon, make sure you have
- C++ 11 compiler
- CMake >= 3.0
- Bash compatible shell (though this is only to automate build processes etc)

And to run tests you will also need python 3

To build `carbon-ir`, in the main directory run
`./build.sh`.

## Running the tests
To run carbon's tests you will first need to build carbon-ir.
Once you have the compiler you can just run `./test.sh` to run all tests
at once and run `./test.sh testX` to run a specific test (change X to the 
number of the test you want to run).

