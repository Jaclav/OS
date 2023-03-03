bits 16
global setInterrupts
setInterrupts:
	; https://wiki.osdev.org/Interrupt_Vector_Table
	; https://wiki.osdev.org/Interrupt_Service_Routines
	push	ebp
	mov		ebp,	esp

	push	ds
	;handle 0 division
	mov		bx,		0
	mov 	ds,		bx
	mov		WORD[ds:bx+2],	cs
	mov		WORD[ds:bx],	divZero
	;handle syscall
	mov		bx,		0x20*4
	mov		WORD[ds:bx+2],	cs
	mov		WORD[ds:bx],	syscall
	pop		ds

	pop 	ebp
	ret

syscall:
	;; system calls
	cmp		ah,		0
	je		.putc
	cmp		ah,		1
	je		.puts
	; if not recognised print message
	mov		bx,		.message
	jmp		.puts
	iret
	.message db "SYSCALL YAY!",0xa,0x0d,0

	.putc:
		;; al is character to put
		xor		ah,		ah
		push	ax
		call 	putc
		add		sp,		2
		iret

	.puts:
		;; bx is address of string to put
		push	bx
		call	puts
		add		sp,		2
		iret

divZero:
	;; zero division handler
	; print message
	mov		cx,		.len
	.loop:
	mov		ah,		0xe
	mov		bx,		.len
	sub		bx,		cx
	mov		al,		cs:.message+bx
	int 	0x10
	loop .loop

	xchg	bx,		bx				; start debug
	;pop		bx					; EIP
	;pop		ds					; CS
	;add 		sp,		2			; EFLAGS

	cmp		esi,	0				; when C divides it uses idiv - 3 byte and divides by esi
	jne 	.Csrc
	add	WORD[esp],	2				; if esi register is zero (it's int 0) then add 2 and 1 byte
	.Csrc:							; if it is 'normal' 1 byte div
	add	WORD[esp],	1

	mov		ax,		1
	iret
	.message db "ERROR: Zero division!",0xa,0x0d
	.len equ $-.message

%include "boot/io.asm"