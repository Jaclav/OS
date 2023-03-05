INTFLAGS=-mgeneral-regs-only -mno-red-zone -mgeneral-regs-only
WFLAGS=-Wno-implicit-function-declaration -Wno-int-conversion -Wall -Wextra -pedantic -Wfatal-errors
CFLAGS=$(WFLAGS) -fno-pie -ffreestanding -m16 -O0 -s -masm=intel -c -std=gnu11 $(INTFLAGS)
SRC=$(wildcard kernel.c kernel/*.c kernel/*.asm)
OBJS=$(SRC:.c=.o) $(SRC:.asm=.o)

BOOT=$(wildcard boot/*.asm)
BINS=$(BOOT:.asm=.bin)

KERNEL_ADDRESS=0x1000
DISK_START=15
KERNEL_SIZE=$(DISK_START)-1
DISK_SECTOR=$(DISK_START)+1	#because sectors are conted from 1
MACROS=-DKERNEL_ADDRESS=$(KERNEL_ADDRESS) -DKERNEL_SIZE=$(KERNEL_SIZE) -DDISK_SECTOR=$(DISK_SECTOR)

run: clean kernel disk
	bochs -q
	#qemu-system-i386 -drive file=bin/OS.img,format=raw,if=floppy,index=0

kernel:$(BINS) $(OBJS)
	ld -T linker.ld -melf_i386 bin/*.o bin/kernel/*.o -o bin/kernel.bin
	cat bin/boot/boot.bin bin/kernel.bin > bin/OS.img

#automatize this
disk: disk/table.bin disk/auto.bin disk/program.bin
	dd if=/dev/zero of=bin/OS.img seek=100 count=1				# create buffor
	dd if=bin/disk/table.bin of=bin/OS.img seek=$(DISK_START)	# write disk table at 14th sector
	cat bin/disk/auto.bin >> bin/OS.img
	cat bin/disk/program.bin >> bin/OS.img

%.bin: %.asm
	nasm $(MACROS) -fbin $< -o bin/$@

%.bin: %.c
	gcc $(CFLAGS) -Os $< -o bin/$<.o
	ld -T linker.ld -melf_i386 bin/$<.o -o bin/$@

%.o:%.c
	gcc $(MACROS) $(CFLAGS) $< -o bin/$@

%.o:%.asm
	nasm $(MACROS) -felf32 $< -o bin/$@

clean:
	rm -rf bin/*
	mkdir -p bin
	mkdir -p bin/boot
	mkdir -p bin/disk
	mkdir -p bin/kernel