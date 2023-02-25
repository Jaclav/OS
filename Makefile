CFLAGS=-Wno-implicit-function-declaration -Wno-int-conversion -Wall -Wextra -pedantic -fno-pie -ffreestanding -Wfatal-errors -m16 -O0 -masm=intel -c -std=gnu11
run: compile
	qemu-system-i386 -drive file=bin/OS.img,format=raw,if=floppy,index=0

compile:
	nasm -f bin boot/boot.asm -o bin/boot.bin
	nasm -f elf32 loader.asm -o bin/loader.o
	gcc $(CFLAGS) kernel/io.c -o bin/io.o
	gcc $(CFLAGS) kernel/string.c -o bin/string.o
	gcc $(CFLAGS) kernel/graphics.c -o bin/graphics.o
	gcc $(CFLAGS) kernel.c -o bin/kernel.o
	ld -T linker.ld -melf_i386 bin/*.o -o bin/kernel.bin

	cat bin/boot.bin bin/kernel.bin > bin/OS.img

bochs: compile
	echo c | bochs -q

clean:
	rm bin/*