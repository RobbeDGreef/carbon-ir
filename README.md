# Carbon Intermediate Representation
The carbon intermediate representation or carbon for short
is a architecture independent optimizer and code generator designed to
easily connect the front end of a compiler to multiple backend architectures.

Carbon can either be used as an standalone compiler via it's own assembly-like
language or linked into your compiler and accessed via its C API.

## Build
Before you build carbon, make sure you have
- C++ 11 compiler
- CMake >= 3.0
- Bash compatible shell (though this is only to automate build processes etc)

And to run tests you will also need python 3

To build the `carbon-ir` compiler, in the main directory run
`./build.sh`.

To build the `libcarbon-ir.a` static library, in the main directory run
`./buildlib.sh`

## Running the tests
To run carbon's tests you will first need to build carbon-ir.
Once you have the compiler you can just run `./test.sh` to run all tests
at once and run `./test.sh testX` to run a specific test (change X to the 
number of the test you want to run).

The C API testing framework is still being worked on so for now you can go into
tests/capi/ and run the makefile to generate the test executables.

## Examples of the language
The carbon-ir language is a very simple and staticly typed.

This function will add 1 and 5 together and return its result

    function i32 main() <global:true>
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

For all the other operations and info check the operations.md in the
docs folder

## Usage of the compiler
The most basic usage of carbon would be:

    carbon-ir ./file-to-compile.ir

This will just generate a binary for the file specified called a.out
in the default architecture (x86). However to change that name you
can utilise the -o flag

    carbon-ir ./file-to-compile -o the-name

If you would like to generate for a different machine type use
the -m flag

    carbon-ir ./file-to-compile -o the-name -m aarch64

Currently the architectures you can compile for are:
x86, z80, aarch64 (armv8) and JVM but please check the machines.md 
file in the docs folder for more information on that.

For more flags check the help flag

    carbon-ir --help

## Usage of the C API
Here follows a simple example of the carbon C API.

    #include <api/carbon.h>
    int main()
    {
        struct carbon carb = init_carbon("test", "x86");
        struct cfunc func = c_create_func(carb, "main", CTYPE_I32), 0, 0);

        c_add_func_attribute(func, "global:true");

        c_push_op(func, INTLIT, 1, -1, c_reg(func, 0, CTYPE_I32), CTYPE_I32);
        c_push_op(func, RETURN, c_reg(func, 0, CTYPE_I32), -1, -1, CTYPE_I32);

        /* Generate all the operations and build the executable */
        c_gen_func(func);
        c_destroy_func(func);
        c_writeassembly(carb, "test.s");
        end_carbon(carb);
    }

The API is very simple and everything can be accessed by including the <api/carbon.h> file.
Most features of the carbon language are included in the API, if not, leave an issue and
I will implement it as fast as I can.