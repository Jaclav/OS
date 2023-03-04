iret
dw 0x55aa
;  name			sector offset
db "disk.bin",		1
db "data.txt",		2
db "program.bin",	3
db 0xa,"Litwo! Ojczyzno moja! Ty jestes jak zdrowie,",0xa,\
	"Ile cie trzeba cenic, ten tylko sie dowie,",0xa,\
	"Kto cie stracil. Dzis pieknosc twa w calej ozdobie",0xa,\
	"Widze i opisuje, bo tesknie po tobie",0xa\

times 512-($-$$) db 'W'
