[bits 16]
org		0x7c00

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
	mov		al,		0x0e		; number of sectors to read - size of kernel
	int		0x13				; BIOS disk
	jc read_disk				; CF=1 if error
	; AL = number of readed sectors

; print number of loaded sectors
;TODO: reverse it
xor		ah,		ah
sectors:
	push	ax
	mov 	bx,		10
	mov		dx,		0
	div		bx					; dx = ax % bx

	add		dx, 	'0'
	push	dx
	call	putc
	add		sp,		2
	pop		ax
	mov		dx,		0
	div		bx
	cmp		al,		0
	jne		sectors

; draw vertical line
mov		cx,		0x270
L1:
mov		ah,		0x0c			; write a pixel
mov		al,		0x0f    		; color
mov		bh,		0x00    		; page
mov		dx,		0x30    		; vertical
int		0x10					; BIOS screen
loop 	L1

push	PROMPT_TEXT
call	puts
add		sp,		4

call 	getChar

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; SET SEGMENTS AND JUMP TO KERNEL
mov		ax,		KERNEL_ADDRESS
mov		ds,		ax
mov		es,		ax
mov		fs,		ax
mov		gs,		ax
mov		ss,		ax

jmp 	KERNEL_ADDRESS:0x0
hlt

%include "boot/io.asm"

START_TEXT db "Starting OS...",0x0a,0x0d,"Loading kernel...",0x0a,0x0d,0x00
PROMPT_TEXT db " sectors readed, press any key to continue...",0x00

times 510 - ($-$$) db 0x90
dw 0xaa55