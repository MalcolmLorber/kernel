;idta.asm sets up all the intterupt entry points
extern default_handler
extern idt_ftoi

;error interrupt entry point, we need to only push the error code details to stack
%macro error_interrupt 1
global interrupt_handler_%1
interrupt_handler_%1:
	push 	dword %1
	jmp 	common_handler
%endmacro

;regular interrupt entry point, need to push interrupt number and other data
%macro regular_interrupt 1
global interrupt_handler_%1
interrupt_handler_%1:
	push 	dword 0
	push 	dword %1
	jmp 	common_handler
%endmacro

;common handler for all interrupts, saves all necessary stack data and calls our c intterupt handler
common_handler:
        push dword ds
        push dword es
        push dword fs
        push dword gs
	pusha
	call default_handler
	popa
        pop dword gs
        pop dword fs
        pop dword es
        pop dword ds
        
	add	esp, 8
	iret

regular_interrupt 0
regular_interrupt 1	
regular_interrupt 2
regular_interrupt 3
regular_interrupt 4
regular_interrupt 5
regular_interrupt 6
regular_interrupt 7
error_interrupt 8
regular_interrupt 9
error_interrupt 10
error_interrupt 11
error_interrupt 12
error_interrupt 13
error_interrupt 14
regular_interrupt 15
regular_interrupt 16
error_interrupt 17
%assign i 18
%rep 12
	regular_interrupt i
%assign i i+1
%endrep
error_interrupt 30
%assign i 31
%rep 225
	regular_interrupt i
%assign i i+1
%endrep

;interrupt setup, adds all of out interrupt handlers to the idt
	global idtsetup
idtsetup:
	%assign i 0
	%rep 256
		push interrupt_handler_%[i]
		push i
		call idt_ftoi
		add esp, 8
	%assign i i+1
	%endrep
	ret
