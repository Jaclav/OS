iret
dw 0x55aa
;  name						 sector    size in sectors
db "boot.bin",0,"       ",		1,				1
db "kernel.bin",0,"     ",		2,				13
db "table.bin",0,"      ",		15,				1
db "auto.bin",0,"       ",		16,				1
db "data.txt",0,"       ",		17,				1
db "programC.bin",0,"   ",		18,				1
db 0

times 512-($-$$) db 'O'