global asmmain
[bits 16]
asmmain:
push ebp
mov  ebp, esp

mov ax, [ebp+8]
call printChar

;;print #2
mov ax, [ebp+12]
call print

mov esp,ebp
pop ebp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; READ DISK_ADDRESS
; reading sectors
DISK_ADDRESS equ 0x2000
mov bx, DISK_ADDRESS
mov es, bx
mov bx, 0x0

mov dh, 0x0                     ; head
mov dl, 0x0                     ; drive
mov ch, 0x0                     ; cylinder
mov cl, 0x10                    ; 16th sector (counted from 1), because at 15th sector of memory disk is located via Makefile

read_disk:
    mov ah, 0x02                ; BIOS read
    mov al, 0x02                ; sectors to read
    int 0x13                    ; BIOS disk
    jc read_disk                ; if error repeat
    ; returns number of readed sectors in AL

mov ax, DISK_ADDRESS
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax

push 'J'
jmp DISK_ADDRESS:0x0

;print readed memory
mov cx, 0
L1:
push cx

mov bx,cx
mov ax, [DISK_ADDRESS+bx]
call printChar

pop cx
inc cx
cmp cx, 1024
jle L1

mov ax, 21
ret

%include "boot/io.asm"