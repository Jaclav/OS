printChar:
;AL - character to print
	mov ah, 0x0e
	int 0x10
	ret

print:
;AX - address of text to print
	mov bx, ax
print_L:
	mov al, [bx]
	cmp al, 0
	je print_exit
	call printChar
	inc bx
	jmp print_L
print_exit:
	ret

getChar
	mov ah, 0x00
	int 0x16
	ret

sleep:
	mov cx, 0
	mov ah, 0x86
sleep_L:
	int 0x15
	inc cx
	cmp cx, 100
	jle sleep_L

	ret