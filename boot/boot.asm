bits 16
%ifidn __?OUTPUT_FORMAT?__, bin
	org		0x7c00
%endif
%include "lib.asm"

mov		ah,		0				; set video mode
mov		al,		0x10			; 640x350 with 16 colors
int		0x10					; BIOS screen

; greeting
push	START_TEXT
call	puts
add		sp,		2

;;;;;;;;;;;;;
; LOAD KERNEL
mov		bx,		KERNEL_ADDRESS
mov		es,		bx				; es:bx is an address to write in memory
mov		bx,		0				; address to write = KERNEL_ADDRESS:0x0

mov		ch,		0				; cylinder
mov		cl,		2				; 2nd sector (counted from 1 - 1st is this bootloader)
mov		dh,		0				; head
mov		dl,		0				; drive

read_disk:
	mov		ah,		2			; read
	mov		al,		KERNEL_SIZE+1; number of sectors to read = disk table + kernel
	int		0x13				; BIOS disk
	jc read_disk				; CF=1 if error
	; AL = number of readed sectors

; print number of loaded sectors
xor		ah,		ah
push	ax
call	puti
add		sp,		2

; draw vertical line
mov		cx,		0x270			; draw 0x270 pixels
L1:
mov		ah,		0x0c			; write a pixel
mov		al,		0x0f    		; color
mov		bh,		0x00    		; page
mov		dx,		0x30    		; vertical position
int		0x10					; BIOS screen
loop 	L1

push	PROMPT_TEXT
call	puts
add		sp,		4

call 	getChar

setSegments KERNEL_ADDRESS
jmp 	KERNEL_ADDRESS:0x200
hlt

%include "boot/io.asm"

START_TEXT db "Starting OS...",0x0a,"Loading kernel...",0x0a,0
PROMPT_TEXT db " sectors readed, press any key to continue...",0

times 510 - ($-$$) db 0x90
dw 0xaa55