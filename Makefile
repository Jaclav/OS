INTFLAGS=-mgeneral-regs-only -mno-red-zone -mgeneral-regs-only
WFLAGS=-Wno-implicit-function-declaration -Wno-int-conversion -Wall -Wextra -pedantic -Wfatal-errors
CFLAGS=$(WFLAGS) -fno-pie -ffreestanding -m16 -O0 -masm=intel -c -std=gnu11 $(INTFLAGS)
SRC=$(wildcard kernel.c kernel/*.c kernel/*.asm)
OBJS=$(SRC:.c=.o)
SOBJS=$(SRC:.asm=.o)

BOOT=$(wildcard boot/*.asm)
BINS=$(BOOT:.asm=.bin)

run: clean $(BINS) $(OBJS) $(SOBJS) disk.bin
	ld -T linker.ld -melf_i386 bin/*.o bin/kernel/*.o -o bin/kernel.bin
	gcc $(CFLAGS) disk2.c -o bin/disk2.o
	ld -T linker.ld -melf_i386 bin/disk2.o -o bin/disk.bin

	cat bin/boot/boot.bin bin/kernel.bin > bin/OS.img
	dd if=/dev/zero of=bin/OS.img seek=100 count=1	# create buffor
	dd if=bin/disk.bin of=bin/OS.img seek=15		# write disk at 15th sector

	bochs -q
	#qemu-system-i386 -drive file=bin/OS.img,format=raw,if=floppy,index=0

%.bin: %.asm
	nasm -fbin $< -o bin/$@

%.o:%.c
	gcc $(CFLAGS) $< -o bin/$@

%.o:%.asm
	nasm -felf32 $< -o bin/$@

clean:
	rm -rf bin/*
	mkdir -p bin
	mkdir -p bin/boot
	mkdir -p bin/kernel