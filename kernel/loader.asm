bits 16
%include "lib.asm"
global load
;! as kernel is in 32 bits functions must be called with 32bit return address - call DWORD
; can also push 0; call func
;TODO: add check if in code is 8E - change of sreg or EA - jmpf and then don't execute
;TODO: how to return if program didn't clean stack!? - add exit function

load:
	push	ebp
	mov		ebp,	esp

	;;;;;;;;;;;
	; READ DISK
	DISK_ADDRESS equ 0x2000
	SECTORS_TO_READ	equ	3
	;save at address es:bx
	mov 	bx,		DISK_ADDRESS
	mov 	es,		bx
	mov 	bx,		0x100

	mov 	dh,		0x0             ; head
	mov 	dl,		0x0             ; drive
	mov 	ch,		0x0             ; cylinder
	mov 	cl,		[ebp+8]			; sector (counted from 1)
	.read_disk:
		mov		ah,		2        	; BIOS read
		mov		al,		[ebp+16]	; sectors to read
		int		0x13                ; BIOS disk
		jc .read_disk               ; CF=1 if error
	; if not all loaded, try again
	cmp		al,		[ebp+16]
	jne		.read_disk

	;;;;;;;;;;;
	; CALL DISK
	mov		ax,		DISK_ADDRESS
	mov		es,		ax

	; set automatic return from code that didn't return on itself
	; put it on first non program position
	mov		ax, 	[ebp+16]
	mov		dx,		0x200
	mul		dx
	add		ax,		0x100
	mov		bx,		ax
	mov		DWORD[es:bx],0x9001f8b8;mov ax,0x1f8 - 504 is error returned
	add		bx,		4
	mov		DWORD[es:bx],0x900002c2;ret 2

	; load at 0x2000:80 program parameters
	mov 	si, 	WORD[ebp+12]	; pointer to parameter string in si
	mov 	di, 	0x80
	.loop:
		mov 	al, 	[si]
		mov 	[es:di],al
		inc 	di
		inc 	si
		cmp 	di, 	0xff
		jge 	.after
		cmp		al, 0
		jne 	.loop
	.after:

	setSegments 	DISK_ADDRESS
	push 	0						; set zero as flag register - for iret
	; set COM header, see header.asm
	mov 	DWORD[0x0],	0x006a9090
	mov 	DWORD[0x4],	0xcf00f9e8
	call	DISK_ADDRESS:0x0		; push flags; push cs; push ip
	mov		dx,		ax				;save return code
	setSegments 	KERNEL_ADDRESS

	pop		ebp
	mov		ax,	dx
	ret
