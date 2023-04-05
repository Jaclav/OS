WFLAGS=-Wno-int-conversion -Wall -Wextra -pedantic -Wfatal-errors
CFLAGS=$(WFLAGS) -fno-pie -ffreestanding -m16 -O0 -s -masm=intel -c -std=gnu11 -Iinclude
INTFLAGS=-mgeneral-regs-only -mno-red-zone

SRC=$(wildcard kernel/*.c kernel/*.asm)
OBJS=$(SRC:.c=.o) $(SRC:.asm=.o)

BOOT=$(wildcard boot/*.asm)
BINS=$(BOOT:.asm=.bin)

KERNEL_ADDRESS=0x1000
DISK_START=18#counted from 0, so first disk sector will be 17 (counted from 1)
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
disk: disk/auto.bin disk/test.bin disk/pic.bin disk/image.bin
	@dd if=bin/disk/auto.bin of=bin/disk/disk.img seek=0	2> /dev/null
	@dd if=/dev/zero of=bin/disk/disk.img seek=100 count=1	2> /dev/null
	@dd if=bin/disk/test.bin of=bin/disk/disk.img seek=2	2> /dev/null
	@dd if=/dev/zero of=bin/disk/disk.img seek=100 count=1	2> /dev/null
	@dd if=bin/disk/pic.bin of=bin/disk/disk.img seek=9		2> /dev/null
	@dd if=/dev/zero of=bin/disk/disk.img seek=100 count=1	2> /dev/null
	@dd if=bin/disk/image.bin of=bin/disk/disk.img seek=14	2> /dev/null

	@dd if=/dev/zero of=bin/OS.img seek=100 count=1			2> /dev/null
	@dd if=bin/disk/disk.img of=bin/OS.img seek=$(DISK_START)	2> /dev/null

%.bin: %.asm
	nasm $(MACROS) -fbin $< -o bin/$@

#BUG: should be optimalized with -Os but then i doesn't work
%.bin: %.c
	gcc -ffunction-sections -fdata-sections -fwhole-program $(CFLAGS) $< -o bin/$<.o
	objdump -D -M i8086 bin/$<.o -M intel > bin/debug/$<.asm
	ld --gc-sections -T disk/linker.ld -melf_i386 bin/$<.o -o bin/$@

%.o:%.c
	gcc $(MACROS) $(CFLAGS) $(INTFLAGS) $< -o bin/$@
	objdump -D -M i8086 bin/$@ -M intel > bin/debug/$<.asm

%.o:%.asm
	nasm $(MACROS) -felf32 $< -o bin/$@

clean:
	@rm -rf bin/*
	@mkdir -p bin
	@mkdir -p bin/boot
	@mkdir -p bin/disk
	@mkdir -p bin/kernel
	@mkdir -p bin/debug
	@mkdir -p bin/debug/kernel
	@mkdir -p bin/debug/disk
