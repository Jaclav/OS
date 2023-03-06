; code of first 0x100 Bytes in program segment
; it calls actual program
; TODO: add parameter like in https://en.wikipedia.org/wiki/Program_Segment_Prefix
bits 16
xchg bx,bx
call 0x100
iret


;00000000  87DB              xchg bx,bx
;00000002  9A00010020        call 0x2000:0x100
;00000007  58                pop ax
;00000008  CF                iret

;00000000  87DB              xchg bx,bx
;00000002  E8FB00            call 0x100
;00000005  CF                iret