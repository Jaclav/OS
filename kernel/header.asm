; code of first 0x100 Bytes in program segment
; it calls actual program
; TODO: add parameter like in https://en.wikipedia.org/wiki/Program_Segment_Prefix
bits 16
xchg bx,bx			; in loader it's replaced with nop nop
push 0				; because C uses 32 bit return, assembly must return ret 2 to pop this
call 0x100
iret

;00000000  87DB              xchg bx,bx
;00000002  6A00              push byte +0x0
;00000004  E8F900            call 0x100
;00000007  CF                iret