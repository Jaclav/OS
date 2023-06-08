WFLAGS=-Wno-int-conversion -Wall -Wextra -pedantic -Wfatal-errors
CFLAGS=$(WFLAGS) -fno-pie -ffreestanding -m16 -O0 -s -masm=intel -std=gnu11 -I../include
run:
	gcc test/test.c $(CFLAGS) -c $< -o ../bin/disk/test.o
	nasm test/ms.asm -felf32 $< -o ../bin/disk/ms.o
	ld -T linker.ld -melf_i386 ../bin/disk/ms.o ../bin/disk/test.o -o ../bin/disk/test.bin