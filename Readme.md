This is a repositiory to hold code about and discuss our independent
study on kernels.

Run cross.sh to set up gcc cross-compiler

The following are commands for compiling and running the assembly:

`nasm boot.asm -f bin -o boot.bin`   
`qemu-system-i386 boot.bin`