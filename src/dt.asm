section .text
	global idt_install
idt_install:
	mov eax, [esp+4]
	lidt [eax]
	ret

	global gdt_install
gdt_install:
	mov eax, [esp+4]
	lgdt [eax]
	ret
