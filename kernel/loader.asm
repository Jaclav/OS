;;
; @file loader-asm.c
; @brief Programs loader
;! @details as kernel is in 32 bits functions must be called with 32bit return address - call DWORD. can also push 0; call func
;;
%include "lib.asm"
bits 16

global load
;;
; @brief load program to segment and executes it
; @param beginSector [ebp+8]
; @param track [ebp+12]
; @param parameter [ebp+16]
; @param size [ebp+20]
; @param segment [ebp+24]
; @return int value returned from program or -ENOEXEC (-8) when error
; @todo add sys_exec(int filename)
; @todo add sys_exit(int code) as subprogram
; @todo add check if in code is 8E = change of sreg or EA = jmpf and then don't execute
; @todo change call far on jmp far and iret to push 0; push callerAddress; push DWORD[0x8]; iret
; @todo add pages int 10h AH=5
; @todo split it to loading and executing
; @todo add to programsMap program's properties
;;
load:
	push	ebp
	mov		ebp,	esp

	; ;;;;;;;;;;
	;* READ DISK
	mov		bx,		[ebp+24]
	mov 	es,		bx				; segment
	mov 	bx,		0x100			; offset
	mov 	dh,		0               ; head
	mov 	dl,		0				; drive
	mov 	ch,		[ebp+12]        ; track
	mov 	cl,		[ebp+8]			; sector (counted from 1)
	mov		ah,		2        		; BIOS read
	mov		al,		[ebp+20]		; sectors to read
	int		0x13                	; BIOS disk
	jc 		.error					; CF = 1 disk reading error
	cmp		al,		[ebp+20]		; AL = number of readed sectors
	jne		.error

	; ;;;;;;;;;;;;;;;;;;;;
	;* PREPARE NEW SEGMENT
	; set automatic return from code that didn't return on itself
	; put it on first non program position - (size*512)+0x100+4
	mov		ax, 	[ebp+20]
	imul	ax,		512
	add		ax,		0x104
	mov		bx,		ax
	mov		DWORD es:[bx],	0x900008b8	;mov ax,0x8 - ENOEXEC
	mov		DWORD es:[bx+4],0x900002c2	;ret 2

	; load at 0x2000:80 program parameters
	mov 	si, 	WORD[ebp+16]		; pointer to parameter string in si
	xor		bx,		bx
	.loop:
		mov 	al, 	[si+bx]
		mov 	es:[bx+0x80],al
		inc 	bx
		cmp 	bx, 	0x7f
		jge 	.after
		cmp		al,		0
		jne 	.loop
	.after:
	mov 	BYTE es:[0xff],	0			;set null character

	; ;;;;;;;;;;;;;;;;;
	; RUN NEW SEGMENT
	setSegments 	es
	; set COM header like in https://en.wikipedia.org/wiki/Program_Segment_Prefix
	;00000000  9090              nop nop
	;00000002  6A00              push byte 0x0 ; because C uses 32 bit return, assembly must return ret 2 to pop this
	;00000004  E8F900            call 0x100
	;00000007  CF                iret
	mov 	DWORD[0x0],	0x006a9090
	mov 	DWORD[0x4],	0xcf00f9e8
	mov 	DWORD[0x8],	.end
	mov 	DWORD[0x12],ebp

	mov		WORD[0x16], 0			; CALL OFFSET
	mov		WORD[0x18], es			; CALL SEGMENT
	mov		sp,		0xffff			; set stack begin on end of segment
	mov		ebp,	0xffff			; set stack begin on end of segment
	push 	0						; set zero as flag register - for iret
	call far [es:0x16]				; push flags; push cs; push ip
	.end:
	; in caller CS
	mov		dx,		ax				;save return code
	mov 	esp,	[0x12]
	setSegments 	cs

	pop		ebp
	mov		ax,	dx
	ret

	.error:
		; return -ENOEXEC
		pop		ebp
		mov		eax,	-8				; ENOEXEC - executable format error
		ret