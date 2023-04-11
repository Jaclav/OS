#!/bin/bash
cd bin/disk
begin=0

echo "iret" > ../fat.asm
echo "dw 0x55aa" >> ../fat.asm
echo ";  name							track	sector    	 	size in sectors" >> ../fat.asm
echo "db \"bootloader\",0,\"     \",		0,		1,				1" >> ../fat.asm
echo "db \"table.bin\",0,\"      \",		0,		2,				1" >> ../fat.asm
echo "db \"kernel.sys\",0,\"     \",		0,		3,				KERNEL_SIZE" >> ../fat.asm

ls | while read a;
do
[[ $a =~ \.bin ]] && mv $a "${a::-4}.com"
done
mv data.com data.txt
mv image.com image.bmp

wc -c *.[^i][^a^\.][^a] | while read a b;
do
	if [ "$b" != "razem" ]; then
		size=`python3 <<< "import math; print(math.ceil($a/512))"`
		echo "Size: $size; begin: $begin" $b
		fill=`python3 <<< "print(' '*$((15-${#b})))"`
		echo "db \"$b\",0,\"$fill\", 1, $((begin+1)), $size" >> ../fat.asm

		dd if=$b of=disk.img seek=$begin	2> /dev/null
		dd if=/dev/zero of=disk.img seek=100 count=1	2> /dev/null
		begin=$((begin+size))
	fi
done
echo "db 0" >> ../fat.asm
echo "times 512-(\$-\$\$) db 0" >> ../fat.asm
cd ../../