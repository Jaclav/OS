KERNEL_ADDRESS equ 0x1000
%macro setSegments 1
	;; set all segments (except cs) to %1
	mov		ax,		%1
	mov		ds,		ax
	mov		ss,		ax
	mov		es,		ax
	mov		fs,		ax
	mov		gs,		ax
%endmacro

%macro dbg 0
	;; MAGIC BREAKEPOINT in bochs, to enable in .bochsrc:
	;; magic_break: enabled=1
	xchg	bx,		bx
	nop
%endmacro