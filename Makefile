CFLAGS=-Wbuiltin-declaration-mismatch -Wall -pedantic -fno-PIC -fomit-frame-pointer -ffreestanding -Wfatal-errors -m16 -Os -s -masm=intel -c
run:
	nasm -f bin boot.asm -o ./bin/boot.bin
	gcc $(CFLAGS) kernel.c -o ./bin/kernel.o
	gcc $(CFLAGS) io.c -o ./bin/io.o
	ld -T linker.ld -melf_i386 bin/*.o -o bin/kernel.bin

	cat ./bin/boot.bin ./bin/kernel.bin > ./bin/OS.img
	#bochs
	echo c | bochs -q

	#qemu
	#dd if=/dev/zero of=./bin/floppy.img bs=1024 count=1440
	#dd if=./bin/OS.bin of=./bin/floppy.img seek=0 count=1 conv=notrunc
	#genisoimage -quiet -V 'MYOS' -input-charset iso8859-1 -o ./bin/myos.iso -b floppy.img -hide floppy.img ./bin/
	#qemu-system-i386 -cdrom ./bin/myos.iso
	qemu-system-i386 -drive file=./bin/OS.img,format=raw,if=floppy,index=0

pendrive:
	nasm -f bin main.asm -o boot.bin
	sudo dd if=bootloader of=/dev/sdb bs=512
