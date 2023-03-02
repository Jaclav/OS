bits 16

push	ebp
mov		ebp,	esp

push	WORD[ebp+8]
call	putc
add		sp,		2

jmp		start
db "DISK INFORMATION: ABC",0x00

start:
push	DWORD START_TEXT
call	puts
add		sp,		2

;;;;;;;;;;;;;;;;;;
;; return procedure
mov		bx,		[ebp+6]
mov		es,		bx
mov		bx,		0x0

; change segment
mov		bx,		[ebp+4]			; offset
mov		ax,		[ebp+6]			; segment
pop		ebp						; get previous frame pointer
mov		ds,		ax
mov		es,		ax
mov		fs,		ax
mov		gs,		ax
mov		ss,		ax

; jump back
mov		[cs:RETURN_ADDRESS],	bx	; offset
mov		[cs:RETURN_ADDRESS+2], 	ax	; segment
mov		esp,	ebp				; reset stack as it was
jmp		far [cs:RETURN_ADDRESS]
hlt

%include "boot/io.asm"

START_TEXT db 0x0a,0x0d,"Disk loaded successfully",0x0a,0x0d,0x00
RETURN_ADDRESS dw 0x0,0x0

times 512 - ($-$$) db 0x90