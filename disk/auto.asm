bits 16
%ifidn __?OUTPUT_FORMAT?__, bin
	org 	0x100
%endif
push	ebp
mov		ebp,	esp

;puts parameter
mov		ah,		1
mov		bx,		0x80
int		0x20

jmp		start
db "DISK INFORMATION: ABC",0x00

start:
mov		ah,		1
mov		bx,		DISK_TEXT
int		0x20

mov		esp,	ebp
pop		ebp
mov 	ax, 	0
ret 	2

DISK_TEXT db 0x0a,"Disk loaded successfully.",0x0a,0x00