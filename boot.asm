org 0x7c00

; set video mode
mov ah, 0x00
mov al, 0x02
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
    mov al, 0x03                ; sectors to read
    int 0x13                    ; BIOS disk
    jc read_disk                ; if error repeat
    ; returns number of readed sectors in AL

; print info
add al, '0'
call printChar

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

%include "io.asm"

START_TEXT db "Starting OS...",0x0a,0x0d,"Loading disk...",0x0a,0x0d,0x00
DISK_TEXT db " sectors readed, press any key to continue...",0x00

times 510 - ($-$$) db 0x90
dw 0xaa55