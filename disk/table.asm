%ifndef DISK_SECTOR
	%define DISK_SECTOR 0
%endif
%ifndef KERNEL_SIZE
	%define KERNEL_SIZE 0
%endif
iret
dw 0x55aa
;  name						 sector    	 size in sectors
db "boot.bin",0,"       ",		1,				1
db "kernel.bin",0,"     ",		2,				KERNEL_SIZE
db "table.bin",0,"      ",		DISK_SECTOR,	1
db "auto.bin",0,"       ",		DISK_SECTOR+1,	1
db "data.txt",0,"       ",		DISK_SECTOR+2,	1
db "programC.bin",0,"   ",		DISK_SECTOR+3,	2
db 0

times 512-($-$$) db 0