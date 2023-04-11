iret
dw 0x55aa
;  name							track	sector    	 	size in sectors
db "bootloader",0,"     ",		0,		1,				1
db "table.bin",0,"      ",		0,		2,				1
db "kernel.sys",0,"     ",		0,		3,				KERNEL_SIZE
db "auto.com",0,"       ",		1,		1,				1
db "data.txt",0,"       ",		1,		2,				1
db "image.bmp",0,"      ",		1,		3,				2
db "pic.com",0,"        ",		1,		5,				4
db "sec.com",0,"        ",		1,		9,				1
db "test.com",0,"       ",		1,		10,				8
db "touch.com",0,"      ",		1,		18,				1
db 0
times 512-($-$$) db 0