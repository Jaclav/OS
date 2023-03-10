INTFLAGS=-mgeneral-regs-only -mno-red-zone -mgeneral-regs-only
WFLAGS=-Wno-implicit-function-declaration -Wno-int-conversion -Wall -Wextra -pedantic -Wfatal-errors
CFLAGS=$(WFLAGS) -fno-pie -ffreestanding -m16 -O0 -s -masm=intel -c -std=gnu11 -Iinclude $(INTFLAGS)
SRC=$(wildcard kernel/*.c kernel/*.asm)
OBJS=$(SRC:.c=.o) $(SRC:.asm=.o)

BOOT=$(wildcard boot/*.asm)
BINS=$(BOOT:.asm=.bin)

KERNEL_ADDRESS=0x1000
DISK_START=16#counted from 0, so first disk sector will be 17 (counted from 1)
KERNEL_SIZE=$(DISK_START)-2#bootloaderr will load KERNEL_SIZE + 1 = 15 sectors
MACROS=-DKERNEL_ADDRESS=$(KERNEL_ADDRESS) -DKERNEL_SIZE=$(KERNEL_SIZE)

run: clean kernel disk
	bochs -q
#qemu-system-i386 -drive file=bin/OS.img,format=raw,if=floppy,index=0

kernel:$(BINS) $(OBJS) disk/fat.bin
#code loaded to memory by bootloader contains disk table and kernel
	ld -T kernel/kernel.ld -melf_i386 bin/kernel/*.o -o bin/kernel.bin
	cat bin/boot/boot.bin bin/disk/fat.bin bin/kernel.bin > bin/OS.img

#automatize this
disk: disk/auto.bin disk/program.bin disk/pic.bin disk/image.bin
	dd if=bin/disk/auto.bin of=bin/disk/disk.img seek=0
	dd if=/dev/zero of=bin/disk/disk.img seek=100 count=1
	dd if=bin/disk/program.bin of=bin/disk/disk.img seek=2
	dd if=/dev/zero of=bin/disk/disk.img seek=100 count=1
	dd if=bin/disk/pic.bin of=bin/disk/disk.img seek=5
	dd if=/dev/zero of=bin/disk/disk.img seek=100 count=1
	dd if=bin/disk/image.bin of=bin/disk/disk.img seek=8

	dd if=/dev/zero of=bin/OS.img seek=100 count=1
	dd if=bin/disk/disk.img of=bin/OS.img seek=$(DISK_START)

%.bin: %.asm
	nasm $(MACROS) -fbin $< -o bin/$@

%.bin: %.c
	gcc $(WFLAGS) -fno-pie -ffreestanding -m16 -s -masm=intel -c -std=gnu11 -Iinclude -Os $< -o bin/$<.o
	ld -T disk/linker.ld -melf_i386 bin/$<.o -o bin/$@

%.o:%.c
	gcc $(MACROS) $(CFLAGS) $< -o bin/$@

%.o:%.asm
	nasm $(MACROS) -felf32 $< -o bin/$@

clean:
	@rm -rf bin/*
	@mkdir -p bin
	@mkdir -p bin/boot
	@mkdir -p bin/disk
	@mkdir -p bin/kernel