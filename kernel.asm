;set video mode
mov ah, 0x00
mov al, 0x02
int 0x10

;set colors
mov ah, 0x0B
mov bh, 0x00
mov bl, 0x08
int 0x10

mov ax, KERNEL_TEXT
call print

main:

    mov ah, 0x00
	int 0x16

    call printChar

    jmp main

hlt

%include "io.asm"

KERNEL_TEXT db "Kernel loaded.",0
;times 510 - ($-$$) db 0x90