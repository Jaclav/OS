pop ax
call printChar
jmp start
db "DISK",0x00
start:
mov ax, START_TEXT
call print

call getChar

KERNEL_ADDRESS equ 0x1000
mov bx, KERNEL_ADDRESS
mov es, bx
mov bx, 0x0

mov ax, KERNEL_ADDRESS
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax

pop ax
add ax, 0x10
jmp KERNEL_ADDRESS:0xf17       ;TODO: this is a nightmare, but works

L1:
jmp L1

hlt

%include "boot/io.asm"

START_TEXT db 0x0a,0x0d,"Disk loaded",0x0a,0x0d,0x00

times 512 - ($-$$) db 0x90