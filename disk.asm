pop ax
call printChar

jmp start
db "DISK INFORMATION: ABC",0x00

start:
mov ax, START_TEXT
call print

;;return procedure
mov bx, KERNEL_ADDRESS
mov es, bx
mov bx, 0x0

;get return address
pop cx
add cx, 7
; change segment
mov ax, KERNEL_ADDRESS
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax

mov [cs:RETURN_ADDRESS], cx                         ;offset
mov [cs:RETURN_ADDRESS+2], WORD KERNEL_ADDRESS      ;segment

jmp far [cs:RETURN_ADDRESS]

L1:
jmp L1

%include "boot/io.asm"

START_TEXT db 0x0a,0x0d,"Disk loaded successfully",0x0a,0x0d,0x00
RETURN_ADDRESS dw 0x0,0x0

times 512 - ($-$$) db 0x90