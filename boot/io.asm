KERNEL_ADDRESS equ 0x1000

debug:
;MAGIC BREAKEPOINT in bochs, to enable in .bochsrc:
;magic_break: enabled=1
	xchg	bx,		bx
	nop
	ret

putc:
	push	bp
	mov		bp,		sp
	push	ax
	push	bx

	mov		al,		[bp + 4]; character
	mov		ah,		0x0e	; TELETYPE OUTPUT
	mov		bh,		0x00	; page
	mov 	bl,		0x0f	; color = white
	int 	0x10			; BIOS screen

	pop 	bx
	pop 	ax
	mov		sp,		bp
	pop		bp
	ret

puts:
	push	bp
	mov		bp,		sp
	push	ax
	push	bx

	mov 	bx,		[bp + 4]
puts_L:
	mov al, [bx]
	cmp al, 0
	je puts_exit

	push	ax
	call	putc
	add		sp,		2

	inc bx
	jmp puts_L
puts_exit:
	pop 	bx
	pop 	ax
	mov		sp,		bp
	pop		bp
	ret

;TODO remove old printChar and print

printChar:
;AL - character to print
	mov ah, 0x0e	;write character
	mov bh, 0x00	;page
	mov bl, 0x0f	;color
	int 0x10
	ret

print:
;AX - address of text to print
	mov bx, ax
print_L:
	mov al, [bx]
	cmp al, 0
	je print_exit
	push bx
	call printChar
	pop bx
	inc bx
	jmp print_L
print_exit:
	ret

getChar:
	mov ah, 0x00
	int 0x16
	ret