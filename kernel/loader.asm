bits 16
%include "lib.asm"
global asmmain
extern putc
extern puts
extern puti
;! as kernel is in 32 bits functions must be called with 32bit return address - call DWORD
; can also push 0; call func

asmmain:
	push	ebp
	mov		ebp,	esp

	push	DWORD[ebp+12]
	call	DWORD puts
	add		esp,	4

	;;;;;;;;;;;
	; READ DISK
	DISK_ADDRESS equ 0x2000
	SECTORS_TO_READ	equ	1
	;save at address es:bx
	mov 	bx,		DISK_ADDRESS
	mov 	es,		bx
	mov 	bx,		0x0

	mov 	dh,		0x0             ; head
	mov 	dl,		0x0             ; drive
	mov 	ch,		0x0             ; cylinder
	mov 	cl,		[ebp+8]			; sector (counted from 1)
	.read_disk:
		mov		ah,		2        	; BIOS read
		mov		al,		SECTORS_TO_READ; sectors to read
		int		0x13                ; BIOS disk
		jc .read_disk               ; CF=1 if error
	; if not all loaded, try again
	cmp		al,		SECTORS_TO_READ
	jne		.read_disk

	;;;;;;;;;;;
	; CALL DISK
	setSegments 	DISK_ADDRESS
	push	'Z'						; give a parameter to program
	push 	0						; set zero as flag register - for iret
	call	DISK_ADDRESS:0x0		; push cs; push ip; with line above can iret
	add		sp,		2
	setSegments 	KERNEL_ADDRESS

	push	WORD 'W'
	call	DWORD putc
	add		sp,		2

	;;;;;;;;;;;;
	; PRINT DISK
	; change segment
	setDS	DISK_ADDRESS
	mov 	bx, 	0
	.print_disk:
		mov		ax,		[ds:bx]
		push	ax
		call	DWORD putc
		add		sp,		2

		inc		bx
		cmp		bx,		520
		jle		.print_disk
	pop 	ds

	pop		ebp
	mov		eax,		43
	ret
