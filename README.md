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

## Example
The carbon-ir language is a very simple and staticly typed.

This function will add 1 and 5 together and return its result

    function i32 main()
    {
        %0 = i32 1
        %1 = i32 5
        %2 = add i32 %0 %1
        return i32 %2
    }

The C equivalent would then be:

    int main()
    {
        return 1 + 5;
    }

However it could also very well be:

    int main()
    {
        int x = 1;
        int y = 5;
        return x + y;
    }

As you can see when you compile C or any other language to carbon-ir 
you don't have to worry about allocating variables and carbon will 
handle all register allocation and spills for you.