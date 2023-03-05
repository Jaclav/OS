iret
dw 0x55aa
;  name			sector offset   size in sectors
db "auto.bin",		1,				1
db "data.txt",		2,				1
db "programC.bin",	3,				1

times 512-($-$$) db 'O'