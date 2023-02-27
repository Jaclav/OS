global asmmain
[bits 16]
asmmain:
push ebp
mov  ebp, esp

mov ax, TEXT
call print

mov ax, [ebp+8]
call printChar

mov ax, [ebp+12]
call printChar

mov ax, [ebp+16]
call printChar

;;print
mov bx, [ebp+20]
L1:
mov ax, [bx]
mov ah, 0
cmp ax, 0
je L2
call printChar
inc bx
jmp L1
L2:

;;print #2
mov ax, [ebp+24]
call print

mov esp,ebp
pop ebp

mov ax, 21
ret

%include "boot/io.asm"

TEXT db "Hello, this is an assembly file!",0x0a,0x0d,0x00