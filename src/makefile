C=i686-elf-gcc

all:
	/usr/bin/nasm -felf32 head.asm -o boot.o
	/usr/bin/nasm -felf32 paging.asm
	/usr/bin/nasm -felf32 idta.asm
	$C *.c -std=gnu99 -ffreestanding -O2 -Wall -Wextra -c
	$C -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib *.o -lgcc

clean:
	rm *.o *.bin
run:
	qemu-system-i386 -kernel myos.bin -serial file:serial.log -hda /dev/null \
	-drive file=/dev/null,if=none,id=drive-sata0-0-0 \
	-device ich9-ahci,id=ahci \
	-device ide-drive,drive=drive-sata0-0-0,bus=ahci.0 \
