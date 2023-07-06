;;
; @file interrupts-asm.c
; @brief Kernel's interruptions
; @details About interrupts:
; @details When interrupt handler is called on stack are: [esp]=old eip [esp+2]=old cs [esp+4] old flags
; @details to use local interrupt's variables DS must be changed to local i.e. KERNEL_ADDRESS
; @details all cs of interrupt handler are KERNEL_ADDRESS - only kernel should handle
; @todo divZero is error handle, not only zero division https://gcc.gnu.org/onlinedocs/gcc/x86-Function-Attributes.html#x86-Function-Attributes see error code
;;
%include "lib.asm"
bits 16
global setInterrupts
global addInterrupt
extern __printf
;;
; @brief Set system interruptions
; @details set syscall and errors handles
;;
setInterrupts:
	push	ebp
	mov		ebp,	esp
	push	ds

	;handle 0 division
	mov		bx,		0
	mov 	ds,		bx
	mov		WORD[ds:bx+2],	KERNEL_ADDRESS
	mov		WORD[ds:bx],	divZero
	;handle syscall
	mov		bx,		0x20*4
	mov		WORD[ds:bx+2],	KERNEL_ADDRESS
	mov		WORD[ds:bx],	syscall

	pop		ds
	mov		esp,	ebp
	pop		ebp
	ret

;;
; @brief add interruption
; @details set *(number*4)=function
; @details set *(number*4+2)=KERNEL_ADDRESS - only kernel can handle interruptions
; @param number new interruption number [ebp+8]
; @param function interruption handler [ebp+12]
;;
addInterrupt:
	push	ebp
	mov		ebp,	esp
	push	ds

	mov		bx,		0
	mov 	ds,		bx
	mov		ax,		[ebp+8]		; set ax as parameter then ax*=4
	mov		bx,		4
	mul		bx
	mov		bx,		ax
	mov		ax,		WORD[ebp+12]
	mov		WORD[ds:bx],	ax
	mov		WORD[ds:bx+2],	KERNEL_ADDRESS

	pop		ds
	mov		esp,	ebp
	pop		ebp
	ret

;;
; @brief System interruption 0x20
; @details it adds:
; @details 0 - putc AL = character
; @details 1 - puts BX = &string
; @details 2 - puti BX = unsigned number
; @details 3 - printf BX = pointer to first parameter in stack, nth parameter is on BX+4n
;;
syscall:
	enter	0,		0
	; system calls
	cmp		ah,		0
	je		.putc
	cmp		ah,		1
	je		.puts
	cmp		ah,		2
	je		.puti
	cmp		ah,		3
	je		.printf
	; if not recognised print message
	setDS KERNEL_ADDRESS
	push	.message
	call 	puts
	add		sp,		2
	pop		ds
	jmp		.exit
	.message db "SYSCALL YAY!",0xa,0

	.putc:
		; al is character to put
		xor		ah,		ah
		push	ax
		call 	putc
		add		sp,		2
		jmp		.exit

	.puts:
		; bx is address of string to put
		push	bx
		call	puts
		add		sp,		2
		jmp		.exit

	.puti:
		; bx is number to print
		push	bx
		call	puti
		add		sp,		2
		jmp		.exit

	.printf:
		push 	WORD 0
		push	bx
		call 	DWORD 	__printf
		jmp 	.exit
	.exit:
		leave
		iret

divZero:
	; zero division handler
	dbg
	setDS KERNEL_ADDRESS
	push	.err
	call 	puts
	add		sp,		2
	pop		ds

	cmp		esi,	0				; when C divides it uses idiv - 3 byte and divides by esi
	jne 	.Csrc
	add	WORD[esp],	2				; if esi register is divider then add 2 and 1 byte
	.Csrc:							; if it is 'normal' div add 2 bytes
	add	WORD[esp],	2

	mov		ax,		1
	iret
	.err db "ERROR: Zero division!",0xa,0

%include "boot/io.asm"