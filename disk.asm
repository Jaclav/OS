bits 16

push	ebp
mov		ebp,	esp

push	WORD[ebp+8]				; print parameter
call	putc
add		sp,		2

jmp		start
db "DISK INFORMATION: ABC",0x00

start:
push	DWORD DISK_TEXT
call	puts
add		sp,		4

int		0x20

;;;;;;;;;;;;;;;;;;
;; return procedure
; change segment
mov		ax,		[ebp+6]			; segment
mov		bx,		[ebp+4]			; offset
pop		ebp						; reset ebp as it was
mov		esp,	ebp				; reset stack as it was
mov		ds,		ax
mov		es,		ax
mov		fs,		ax
mov		gs,		ax
mov		ss,		ax

; jump back
push	0
push	ax						; segment
push	bx						; offset
iret
hlt

%include "boot/io.asm"

DISK_TEXT db 0x0a,0x0d,"Disk loaded successfully",0x0a,0x0d,0x00

times 512 - ($-$$) db 0x90