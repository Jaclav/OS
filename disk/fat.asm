%include "lib.asm"
%define LOADED_END KERNEL_SIZE+2	;Sector after loaded stuff KERNEL_SIZE + 1 (boot sector) + 1 (table)
iret
dw 0x55aa
;  name						 sector    	 size in sectors
db "bootloader",0,"     ",		1,				1
db "table.bin",0,"      ",		2,				1
db "kernel.sys",0,"     ",		3,				KERNEL_SIZE
db "auto.com",0,"       ",		LOADED_END+1,	1
db "data.txt",0,"       ",		LOADED_END+2,	1
db "c.com",0,"          ",		LOADED_END+3,	4
db "pic.com",0,"        ",		LOADED_END+7,	5
db "image.bmp",0,"      ",		LOADED_END+12,	2
db 0

times 512-($-$$) db 0