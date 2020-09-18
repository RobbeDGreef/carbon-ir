# Supported output machines

Currently the x86 architecture is the most complete and
supported. The other targets currently are Z80 and 
aarch64 but are nowhere near completion. 

You might notice that the arch/ folder also contains
a JVM folder, this was just a test to compile carbon
to a java VM however as expected that is not possible due
to java's pointer abstractions. I left it in in case 
it might interest someone.

## X86
The x86 generator will generate NASM assembly and link it
with gcc.