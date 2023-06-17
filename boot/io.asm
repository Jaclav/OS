bits 16

; ABI:
	;; parameters are on stack - first parameter is on top
	;; after calling caller must clear stack
	;; function must leave CPU as it was before call
	;; use stack frames
	;; stack e.g. [LOCAL VARIABLES] [USED REGISTERS] [FRAME POINTER i.e. bp] [RETURN ADDRESS] [PARAMETERS]

putc:
	;; write single character
	enter 	0,		0
	push	ax
	push	bx

	cmp		al,		0x0a	; \n = \r\n
	jne		.normal
	mov		al,		0x0d	; character
	mov		ah,		0x0e	; TELETYPE OUTPUT
	mov		bh,		0x00	; page
	mov 	bl,		0x0f	; color = white
	int 	0x10			; BIOS screen

	mov		al,		0x0a	; character
	mov		ah,		0x0e	; TELETYPE OUTPUT
	mov		bh,		0x00	; page
	mov 	bl,		0x0f	; color = white
	int 	0x10			; BIOS screen

	;;clear buffor from colors
	;TODO make it customizable
	mov		eax,	0x920
	mov    	ecx,	0xff
	mov    	ebx,	0x7		;default color - dark gray
	int    	0x10
	jmp 	.end

	.normal:
	mov		al,		[bp + 4]; character
	mov		ah,		0x0e	; TELETYPE OUTPUT
	mov		bh,		0x00	; page
	mov 	bl,		0x0f	; color = white
	int 	0x10			; BIOS screen

	.end:
	pop 	bx
	pop 	ax
	leave
	ret

puts:
	;; write string
	enter 	0,		0
	push	ax
	push	bx

	mov		ax,		0
	mov 	bx,		[bp + 4]

	.loop:
	mov		al,		[bx]
	cmp		al,		0
	je		.after
	push	ax
	call	putc
	add		sp,		2
	inc 	bx
	jmp 	.loop

	.after:
	pop 	bx
	pop 	ax
	leave
	ret

puti:
	;; write int
	enter 	0,		0
	push	ax
	push	bx
	push	cx
	push	dx

	mov		ax,		[bp + 4]
	push	0					; end of digits to print

	.loop:
	mov 	bx,		10
	mov		dx,		0
	mov		cx,		ax
	div		bx					; dx = ax % 10
	mov		ax,		cx
	;; push digit in ascii on stack
	add		dx, 	'0'
	push	dx

	mov		dx,		0
	div		bx					; ax = ax / 10
	cmp		al,		0
	jne		.loop

	.loop2:
	pop		ax
	cmp		ax,		0
	je		.after
	push	ax
	call	putc
	add		sp,		2
	jmp		.loop2

	.after:
	pop		dx
	pop		cx
	pop		bx
	pop		ax
	leave
	ret

getChar:
	mov ah, 0x00
	int 0x16
	ret