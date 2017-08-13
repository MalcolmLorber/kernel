This is a repositiory to hold code about and discuss our independent
study on kernels.

Run cross.sh to set up gcc cross-compiler.

Install qemu (or similar emulator) if you plan to run the kernel, as this has only been tested on virtual devices.

# Building and running the kernel
Navigate to the src/ folder and run `make`, followed by `make run` to run. For now use the bootloader integrated into qemu (or emulator of your choice).

On the master branch the kernel consists of an interactive basic terminal, which supports a stack-based calculator and writing to serial output. Process management is WIP.

# Building the Bootloader (NOTE: WIP)
The following are commands for compiling and running the assembly:

`nasm boot.asm -f bin -o boot.bin`   
`qemu-system-i386 boot.bin`

To run the kernel, run these commands from the src directory

`make`   
`qemu-system-i386 -kernel myos.bin -serial file:serial.log`
