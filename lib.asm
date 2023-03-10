; usefull macros library
%ifndef KERNEL_ADDRESS
	%define KERNEL_ADDRESS 0
%endif

%ifndef KERNEL_SIZE
	%define KERNEL_SIZE 0
%endif

%macro setDS 1
	;; set ds safely
	;;! ds must be later popped
	push	ds
	mov		ax,		%1
	mov		ds,		ax
%endmacro

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