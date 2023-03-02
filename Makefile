CFLAGS=-Wno-implicit-function-declaration -Wno-int-conversion -Wall -Wextra -pedantic -fno-pie -ffreestanding -Wfatal-errors -m16 -O0 -masm=intel -c -std=gnu11
CSRC=$(wildcard *.c kernel/*.c)
OBJS=$(CSRC:.c=.o)

BOOT=$(wildcard boot/*.asm)
BINS=$(BOOT:.asm=.bin)

run: clean $(BINS) $(OBJS)
	nasm -fbin disk.asm -o bin/disk.bin
	nasm -felf32 loader.asm -o bin/loader.o
	ld -T linker.ld -melf_i386 bin/*.o bin/kernel/*.o -o bin/kernel.bin

	cat bin/boot/boot.bin bin/kernel.bin > bin/OS.img
	dd if=/dev/zero of=bin/OS.img seek=100 count=1	# create buffor
	dd if=bin/disk.bin of=bin/OS.img seek=15		# write disk at 15th sector

	bochs -q
	#qemu-system-i386 -drive file=bin/OS.img,format=raw,if=floppy,index=0

%.bin: %.asm
	nasm -f bin $< -o bin/$@

%.o:%.c
	gcc $(CFLAGS) $< -o bin/$@

clean:
	rm -rf bin/*
	mkdir -p bin
	mkdir -p bin/boot
	mkdir -p bin/kernel