# What is libsdcc?

`libsdcc` is the glue betweeen the SDCC C compiler and the
Z80 processor. Z80 lacks instructions for integer multiplication 
and division, and for handling long integers and floating point 
numbers.

To mitigate it, the SDCC C compiler replaces these non-existing
instructions with calls to special functions (such as: `__mulint`).
Invisible to you, the linker then links these special functions 
with your code.

This works in the SDCC realm, but if you prevent the compiler to link
default SDCC libraries (by using directives `--nostdlib`, `--nostdinc`, 
and `--no-std-crt0`) then you need to provide these special functions,
and the `libsdcc` does that.