db 'X'
jmp start
db "DISK",0x00
start:
mov ax, START_TEXT
call print

L1:
jmp L1

hlt

%include "boot/io.asm"

START_TEXT db "Disk loaded",0x0a,0x0d,0x00

times 512 - ($-$$) db 0x90