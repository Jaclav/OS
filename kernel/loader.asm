bits 16
KERNEL_ADDRESS equ 0x1000
global asmmain
extern abc
extern putc
extern puts
extern puti
;! as kernel is in 32 bits functions must be called with 32bit address - call DWORD
; can also push 0; call func

asmmain:
	push	ebp
	mov		ebp,	esp

	call	DWORD 	abc

	push	WORD[ebp+8]
	call	DWORD putc
	add		esp,	2

	push	DWORD[ebp+12]
	call	DWORD puts
	add		esp,	4

	;;;;;;;;;;;
	; READ DISK
	DISK_ADDRESS equ 0x2000
	;save at address es:bx
	mov 	bx,		DISK_ADDRESS
	mov 	es,		bx
	mov 	bx,		0x0

	mov 	dh,		0x0             ; head
	mov 	dl,		0x0             ; drive
	mov 	ch,		0x0             ; cylinder
	mov 	cl,		0x10            ; 16th sector (counted from 1), because at 15th sector of memory disk is located via Makefile

	.read_disk:
		mov		ah,		0x02        ; BIOS read
		mov		al,		0x02        ; sectors to read
		int		0x13                ; BIOS disk
		jc .read_disk                ; CF=1 if error
		; AL = number of readed sectors

	; print number of loaded sectors
	xor		ah,		ah
	push	DWORD eax
	call	DWORD puti
	add		sp,		4

	;;;;;;;;;;;
	; CALL DISK
	; change segments
	mov		ax,		DISK_ADDRESS
	mov		ds,		ax
	mov		es,		ax
	mov		fs,		ax
	mov		gs,		ax
	mov		ss,		ax
	; call program
	push	'Z'						; give a parameter to program
	push 	0						; set zero as flag register - for iret
	call	DISK_ADDRESS:0x0		; push cs; push ip; with line above can iret
	add		sp,		2
	; reset segments
	mov		ax,		KERNEL_ADDRESS
	mov		ds,		ax
	mov		es,		ax
	mov		fs,		ax
	mov		gs,		ax
	mov		ss,		ax

	push	WORD 'W'
	call	DWORD putc
	add		sp,		2

	;;;;;;;;;;;;
	; PRINT DISK
	; change segment
	push 	ds
	mov		ax,		DISK_ADDRESS
	mov		ds,		ax
	mov bx, 0
	.print_disk:
		mov		ax,		[ds:bx]
		push	ax
		call	DWORD putc
		add		sp,		2

		inc		bx
		cmp		bx,		600
		jle		.print_disk
	; restore segment
	pop 	ds

	pop		ebp
	mov		eax,		43
	ret
