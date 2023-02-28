bits 16
global asmmain

asmmain:
push ebp
mov  ebp, esp

mov ax, [ebp+8]
call printChar

mov ax, [ebp+12]
call print

mov esp,ebp
pop ebp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; READ DISK_ADDRESS
; reading sectors
DISK_ADDRESS equ 0x2000
;save at address es:bx
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

;change segment
mov ax, DISK_ADDRESS
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax

; push return address
call getAddress
getAddress:
pop cx                          ;get current address
push cx                         ;push it
push 'J'						;give a parameter to program
jmp DISK_ADDRESS:0x0

mov ax,'W'
call printChar

;;  print readed memory
; change segment
mov ax, DISK_ADDRESS
mov ds, ax

mov cx, 0
L1:
push cx

mov bx, cx
mov ax, [ds:bx]
call printChar

pop cx
inc cx
cmp cx, 1024
jle L1

;restore segment
mov ax, KERNEL_ADDRESS
mov ds, ax

mov ax, 22
ret

%include "boot/io.asm"