run:
	nasm -f bin boot.asm -o ./bin/boot.bin
	nasm -f bin kernel.asm -o ./bin/kernel.bin
	cat ./bin/boot.bin ./bin/kernel.bin > ./bin/OS.bin
	#bochs
	echo c | bochs -q

	#qemu
	#dd if=/dev/zero of=./bin/floppy.img bs=1024 count=1440
	#dd if=./bin/OS.bin of=./bin/floppy.img seek=0 count=1 conv=notrunc
	#genisoimage -quiet -V 'MYOS' -input-charset iso8859-1 -o ./bin/myos.iso -b floppy.img -hide floppy.img ./bin/
	#qemu-system-i386 -cdrom ./bin/myos.iso

pendrive:
	nasm -f bin main.asm -o boot.bin
	sudo dd if=bootloader of=/dev/sdb bs=512
