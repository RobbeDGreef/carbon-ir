
## Binary operations
### add
REG = add TYPE REG REG
eg.
    %2 = add i32 %0 %1

The add operation will add the contents of the first register with the contents
of the second register and store the result in the returning register.
The types of all registers must be the same and equal to the type specified after
the instruction. If the returning register is initialized here it's type will
be set to the type specified after the operation.

### sub

### mul

### div

## Memory operations
### alloca
