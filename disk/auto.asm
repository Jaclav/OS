bits 16
push	ebp
mov		ebp,	esp

mov		ah,		0
mov		al,		[ebp+10]
int		0x20

jmp		start
db "DISK INFORMATION: ABC",0x00

start:
mov		ah,		1
mov		bx,		DISK_TEXT
int		0x20

mov		esp,	ebp
pop		ebp
iret
hlt

DISK_TEXT db 0x0a,"Disk loaded successfully.",0x0a,0x00

times 512 - ($-$$) db 0x90

db "Litwo! Ojczyzno moja! Ty jestes jak zdrowie,",0xa,\
	"Ile cie trzeba cenic, ten tylko sie dowie,",0xa,\
	"Kto cie stracil. Dzis pieknosc twa w calej ozdobie",0xa,\
	"Widze i opisuje, bo tesknie po tobie",0xa

times 1024 - ($-$$) db '.'