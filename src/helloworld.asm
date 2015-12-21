	;;  compile with nasm -felf32 hello.asm

	Section .text
	global main
main:
        mov eax, 4
	int 0x80
        mov eax, 5
	int 0x80
