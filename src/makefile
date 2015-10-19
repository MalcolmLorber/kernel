C=i686-elf-gcc

all:
	/usr/bin/nasm -felf32 head.asm -o boot.o
	/usr/bin/nasm -felf32 paging.asm
	/usr/bin/nasm -felf32 idta.asm
	$C *.c -std=gnu99 -ffreestanding -O2 -Wall -Wextra -c
	$C -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib *.o -lgcc

clean:
	rm *.o *.bin
