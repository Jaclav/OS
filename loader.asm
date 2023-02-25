global asmmain
[bits 16]
asmmain:

mov ax, TEXT
call print
ret

%include "boot/io.asm"

TEXT db "Hello, this is an assembly file!",0x0a,0x0d,0x00