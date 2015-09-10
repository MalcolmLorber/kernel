	[bits 16]	; 16 bit real mode
	[org 0x7C00]	; MBR is loaded at 0x7C00

	jmp 0x0000:st 		;cs=0x0000, for consistency
st:
	xor 	ax, ax		;clearing registers
	mov	ds, ax
	mov	es, ax

	mov	dl, 0x80	; HDD1
	xor	dh, dh		; force 0
	mov	bx, 0x7e00	; read to 7e00, after bootloader
	mov	al, 1		; num of sectors to read
	mov	cx, 0x0002	; floppy cylinder
	call 	read_sectors_16	; read with above args
	jnc	0x7e00		; jump to actual code
halt:
	cli			; halt if failed, loop
	hlt
	jmp halt

read_sectors_16:
	pusha
	mov si, 0x02    ; maximum attempts - 1
.top:
	mov ah, 0x02    ; read sectors into memory (int 0x13, ah = 0x02)
	int 0x13
	jnc .end        ; exit if read succeeded
	dec si          ; decrement remaining attempts
	jc  .end        ; exit if maximum attempts exceeded
	xor ah, ah      ; reset disk system (int 0x13, ah = 0x00)
	int 0x13
	jnc .top        ; retry if reset succeeded, otherwise exit
.end:
	popa
	retn

read_fail db 'Failure to read from disk.', 13, 10, 0
	; $ is current line, $$ is first line, db 0 is a 00000000 byte
	; So, pad the code with 0s until you reach 510 bytes
times 510 - ($ - $$) db 0
; Fill last two bytes (a word) with the MBR signature 0xAA55
	dw 0xaa55

	
ls:	mov si, str1	; pointer to string in si
	call PrintStr	; print string
	jmp Main			;
	hlt

	
PrintStr:	; print string at SI
nextChar:
	mov al, [si]	; grab next char
	or al, al	; if char is null term
	jz exit		; then return
	call PrintChar	; else print char
	inc si		; inc pointer to next char
	jmp nextChar	; loop
	exit:
	ret

PrintChar:	; print char at AL
	mov ah, 0x0e	; teletype mode
	mov bh, 0x00	; page zero
	mov bl, 0x07	; light gray
	int 0x10	; print character
	ret

PrintHexStr: 			; print <ax> bytes starting at [si]
	mov di, si		; initilize the loop registers
	mov dx, si
	add dx, ax
nexthchar:
	cmp di, dx
	jge exith 		; NO IDEA HOW THIS WORKS
	mov al, BYTE [di]	; grab next char
	call PrintHexChar	; else print char
	inc di
	jmp nexthchar	; loop
	exith:
	ret

PrintHexChar:
	;; call PrintChar
	xor ebx, ebx
	mov cl, al
	mov bl, al
	and bl, 0xf0
	shr bl, 4
	mov al, BYTE  [hexChars + ebx]
	call PrintChar
	mov bl, cl
	and bl, 0x0f
	mov al, BYTE  [hexChars + ebx]
	call PrintChar
	ret

	
str1 db 'Hello World', 0	; Hello World
str2 db 'WHY IS THIS', 0

hexChars db '0123456789abcdef'		; Used for hex formatting
gdtr:	dw 48
	dw 0x0
	dw gdt
Main:	;main section
	nop
	nop
	mov si, str2
	call PrintStr
	mov si, str1
	mov ax, 10
	call PrintHexStr	; print string
	mov ax, 0x2401
	int 0x15
	;; now to try protected mode...
	cli

setGdt:
	;xor   eax, eax
	;mov   ax, ds
	;shl   eax, 4
	;add   eax, gdt
	;jp $			
	;mov   [gdtr + 2], eax
	;jp $			;
	;mov   eax, gdt_end
	;jp $			
	;sub   eax, gdt			
	;mov   [gdtr], ax
	lgdt [gdtr]		
	mov eax, cr0
	or eax, 1
	mov cr0, eax

	jmp 08h:PMain

gdt:
	times	16	db	0
	dw	0xffff				; segment limit
.src:
	dw	0
	db	2
	db	0x93				; data access rights
	dw	0
	dw	0xffff				; segment limit
.dest:
	dw	0
	db	0x10				; load protected-mode kernel to 100000h
	db	0x93				; data access rights
	dw	0
	times	15	db	0
gdt_end:db 0	
;; .null:	times	8	db	0
;; .code:	dw	0x0400		; limit (4MiB)
;; 	dw	0x0000		; base 0-15
;; 	db	0x00		; base 16-23
;; 	db	0x9A		; access byte
;; 	db	0xC0		; flags/limit 16-19
;; 	db	0x04		; base 24-31
;; .data:	dw	0x0400
;; 	dw	0x0000
;; 	db	0x00
;; 	db	0x92
;; 	db	0xC0
;; 	db	0x08
;; .tss:	dw	0x0400
;; 	dw	0x0000
;; 	db	0x00
;; 	db	0x89
;; 	db	0xC0
;; 	db	0x0C
PMain:
	[bits 32]
	mov si, str1
	call PrintStr
	nop
	nop
	hlt
	jp $
