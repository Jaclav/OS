org 0x7c00

; set video mode
mov ah, 0x00
mov al, 0x10    ; 640x350 with 16 colors
int 0x10

; greeting
mov ax, START_TEXT
call print

; reading sectors
mov bx, 0x1000
mov es, bx
mov bx, 0x0

mov dh, 0x0                     ; head
mov dl, 0x0                     ; drive
mov ch, 0x0                     ; cylinder
mov cl, 0x02                    ; 2nd sector (counted from 1)

read_disk:
    mov ah, 0x02                ; BIOS read
    mov al, 0x20                ; sectors to read
    int 0x13                    ; BIOS disk
    jc read_disk                ; if error repeat
    ; returns number of readed sectors in AL

; print number of loaded sectors
push ax
mov bx, 10
mov dx, 0
div bx,
mov cx, dx                      ;cx = ax % 10

pop ax
sub ax, cx
mov bx, 10
mov dx, 0
div bx                          ;ax = (ax-ax %10)/10=(ax-cx)/bx
add al, '0'
call printChar

mov ax, cx
add ax, '0'
call printChar

;add vertical line
mov cx, 0x00
L1:
mov ah, 0x0c
mov al, 0x0f    ;color
mov bh, 0x00    ;page
inc cx
mov dx, 0x30    ;vertical
int 0x10
cmp cx, 0x270
jle L1

mov ax, DISK_TEXT
call print

call getChar

;reset
mov ax, 0x1000
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax

jmp 0x1000:0x0
hlt

%include "boot/io.asm"

START_TEXT db "Starting OS...",0x0a,0x0d,"Loading disk...",0x0a,0x0d,0x00
DISK_TEXT db " sectors readed, press any key to continue...",0x00

times 510 - ($-$$) db 0x90
dw 0xaa55