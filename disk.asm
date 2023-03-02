bits 16
push	ebp
mov		ebp,	esp

push	WORD[ebp+10]				; print parameter
call	putc
add		sp,		2

jmp		start
db "DISK INFORMATION: ABC",0x00

start:
push	DWORD DISK_TEXT
call	puts
add		sp,		4

int		0x20

mov		esp,	ebp
pop		ebp
iret
hlt

%include "boot/io.asm"

DISK_TEXT db 0x0a,0x0d,"Disk loaded successfully",0x0a,0x0d,0x00

times 512 - ($-$$) db 0x90