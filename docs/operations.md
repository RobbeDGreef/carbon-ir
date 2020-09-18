# Simple carbon-ir documentation
## Basic
### Registers and integer literls
Carbon makes use of a unlimited amount of registers. The operations in carbon
can only accept registers as arguments, not plain integers. 
You can of course store a integer in a register first like this:

REG = TYPE INTLIT-VALUE

e.g.

    %0 = i32 3

Don't worry about the overhead, carbon will handle and optimize that for you
(or at least that's the goal).

### Comments
Carbon uses line comments with the same syntax as python. The hashtag (pound) symbol (#) 
is used to denote a comment.

e.g.

    %0 = i32 5      # This line will be executed but the next one won't
    # %1 = i32 1    

## Binary operations
The binary operations currently are: `add` `sub` `mul` `div` and `mod`

REG = OPERATION TYPE REG REG
e.g.

    %2 = add i32 %0 %1

The binary operations will store the result of their operation in the returning
register. The types of all registers must be the same and equal to the type specified after the instruction. If the returning register is initialized here, it's type will
be set to the type of the other registers.

### add
The add operation will add the content of the first register to that of the second register.

C equivalent: `a = b + c`;

### sub
The sub operation will subtract the content of the first register with that of the second register.

C equivalent: `a = b - c`;

### mul
The mul operation will multiply the content of the first register with that of the
second register.

C equivalent: `a = b * c`;

### div
The div operation will divide the content of the first register with that of the
second register.

C equivalent: `a = b / c`;

### mod
The mod operation will divide the content of the first register with that of the second
register and return the remainder to the returning register.

C equivalent: `a = b % c`;

## Memory operations
### alloca
The alloca keyword will allocate a specified amount of entries on the stack with the size
specified by the type requested. 

e.g.

    %0 = i32 1
    %1 = alloca i32 %0

This example would allocate the space for 1 i32 on the stack (4 bytes).
The alloca operation then stores a pointer to the start of the array in the returning
register. The pointer's type will be the same specified by alloca (i32 in the example).

### store
The store operation can be used to store a value in the memory pointed to by a pointer.

e.g.

    %0 = i32 1
    %1 = alloca i32 %0
    %2 = i32 5
    store i32* %1 %2

This example will first create a pointer that points to a location on the stack, 
and then load the value 5 into that location.

### load
The load operation will load a value pointed to by a pointer into a register.

e.g.

    %0 = i32 1
    %1 = alloca i32 %0
    %2 = i32 5
    store i32* %1 %2
    
    %3 = load i32* %1

We built on the example just above and now the register 3 will hold the value 5.

### Global arrays (and variables)
Global identifiers always start with an at (@) symbol followed by the identifier itself.
Global arrays syntax can be described as follows:

@globalname = [TYPE AMOUNT] OPTIONAL-INITIALIZER-LIST

e.g.

    @globarray = [i32 4] 1 2 3 4

This example will allocate 4 32 bit integers and initialize them with 1, 2, 3 and 4.

### Strings
Strings can be used in an initializer list as shown below.

e.g.

    @str1 = [i8 13] "Hello world" 10 0

## Functions
### Defining a function
The function definition syntax is as follows.

function TYPE NAME(OPTIONAL-ARGUMENTS) <OPTIONAL-ATTRIBUTES>
{
}

e.g.

    function i32 main(i32 %0 i8* %1) <global:true>
    {
    }

The example shows a typical main function.

### return keyword
The return keyword accepts a register as argument to return from the function.

e.g.

    %0 = i32 5
    return i32 %0

The type of the return, the type of the function and the type of the register must
all match.

### call operation
The call operation can invoke a specified function and store its result.

e.g.

    %1 = i32 7
    %2 = call i32 foo(%0)

In this example the function foo is called with one argument (%0) and the result of 
the function will be stored in register 2. The type of the function and the call 
operation have to match.

## Flowcontrol and comparison
### Labels
Labels can be used by control flow operations like jmp or jmpcmp to "jump" to the
label and start executing code from there.

Defining a label is as easy as writing 
LABELNAME:

e.g.

    labelname:
    jmp labelname

This example will create an infinite loop (see the jmp operation section)

### cmp operation
The cmp operation will compare the first regsiter with the second and will, based on
the comparison operation specified, store 1 (true) or 0 (false) in the resulting regsiter

e.g.

    %0 = i32 5
    %1 = i32 2
    %2 = cmp g i32 %0 %1

Register 2 will hold (1) because the value in register 0 is greater than the value
in register 1.

The comparison operations are `eq` `neq` `l` `le` `g` `ge`
meaning: equal, not equal, less than, less than or equal to, greater than, greater than or equal to, respectively.

### jmp operation
The jmp operation will just move the instructions execution point to the specified label.

e.g.

    label0:
    jmp label0

The example above will generate an infinite loop

### jmpcond operation
This operation is an (as the name suggests) jump and comparison operation in one.
it will preform a comparison and jump to the specified label if the condition
evaluates to true.

see the cmp operation section for more details on the comparison features.

e.g.

    %0 = i32 4
    %1 = i32 50
    jmpcmp neq %0 %1 label0

    # this code will not be executed
    %0 = i32 1
    
    label0:
    # Code that actually will be executed here
