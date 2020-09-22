# The C API
The C API is still in its very early stages and not complete
but you can help expand it.


Currently all API functions, structures and global variables are defined
in the <api/carbon.h> file.

## Basic example
A basic example of the usage of the carbon C API.
You can find more examples in the tests/capi/ folder.

    #include <api/carbon.h>
    int main()
    {
        struct carbon carb = init_carbon("test", "x86");
        struct cfunc func = c_create_func(carb, "main", CTYPE_I32), 0, 0);

        c_add_func_attribute(func, "global:true");

        c_push_op(func, INTLIT, 1, -1, c_reg(func, 0, CTYPE_I32), CTYPE_I32);
        c_push_op(func, RETURN, c_reg(func, 0, CTYPE_I32), -1, -1, CTYPE_I32);

        c_gen_func(func);
        c_destroy_func(func);
        c_writeassembly(carb, "test.s");
        end_carbon(carb);
    }

## Structures
### struct carbon
The most important structure is the carbon structure. It holds the information
about the current interface between your compiler and carbon. 

It can be created using the `init_carbon()` function as follows.

    struct carbon carb = init_carbon("infile-name", "x86");

The init functions takes two arguments, the first is the input file name. This is
just for housekeeping (this way the generated assembly code can reference your
source input source code file), you can leave it empty ("") if you want.
The second one is the type of architecture you want to generate a binary for. 
The possible architectures are defined in machine.md in the docs/ folder.

At the end of its life you may want to destroy the carbon structure so that 
its recourses can be freed.

    end_carbon(carb);

### struct cfunc
The function struct is crutial to generate operations and actual code. Use the 
`c_create_func()` to create one.

    struct cfunc func = c_create_func(carb, "func-name", CTYPE_I32, argamount, arglist);

This function takes quite a few arguments. The first is the carbon structure, the second is the name for the function, the third is its return type (see struct ctype for more info). Finally the last two are used to pass the amount of arguments and its types to the
function. The first one being the amount and the second a pointer to an array of struct
ctypes. Carbon will try to read the specified amount of arguments from the pointer.
You can always leave the last two variables to zero if the function doesn't take any 
arguments.

### struct ctype
Types in the C API are represented in the ctype struct. The struct currently
has three fields

    struct ctype
    {
        int byte_size;      /* The amount of bytes the type occupies in memory */
        int ptr;            /* If this is a pointer and if so how deep it references */
        int is_float;       /* If this type is a float or an integer */
    };

Some default types are already defined for you in the carbon.h file.
see.

    CTYPE_I64
    CTYPE_I32
    CTYPE_I16
    CTYPE_I8

And more will be added in the future.

## Functions
@todo