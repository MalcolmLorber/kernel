;--------------- PRELIMINARY SETUP ---------------;
	[BITS 16]	; 16 bit real mode
	[ORG 0x7C00]	; MBR is loaded at 0x7C00 
;---------------- BOOTLOADER CODE ----------------;
ls:	MOV SI, str1	; pointer to string in SI
	CALL PrintStr	; print string
	;MOV ebx, 1000		;start at 10000
	;SUB ebx, 1		;down 1 each time
	;NOP			;waste time
	;JZ 6			;for loop
	;JMP ls			; 
	HLT
;---------------- SCREEN FUNCTIONS ---------------;
PrintStr:	; print string at SI
nextchar:
	MOV AL, [SI]	; Grab next char
	OR AL, AL	; if char is null term
	JZ exit		; then return
	CALL PrintChar	; else print char
	INC SI		; inc pointer to next char
	JMP nextchar	; loop
	exit:
	RET

PrintChar:	; print char at AL
	MOV AH, 0x0E	; Teletype Mode
	MOV BH, 0x00	; Page zero
	MOV BL, 0x07	; Light Gray
	INT 0x10	; Print Character
	RET
;------------------ DATA BLOCK ------------------;

str1 db 'Hello World', 0		;stuff
;-------------- PADDING / SIGNATURE -------------;
	; $ is current line, $$ is first line, db 0 is a 00000000 byte
	; So, pad the code with 0s until you reach 510 bytes
TIMES 510 - ($ - $$) DB 0
; Fill last two bytes (a word) with the MBR signature 0xAA55
	DW 0xAA55

Main:	;main section
