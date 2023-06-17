#!/bin/bash
# it is assumed that i track = 37 sectors
cd bin/disk
begin=0
track=1

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

printf "File sections bytes track sector" | awk '{ printf "%-10s %-10s %-10s %-10s %-10s\n", $1, $2, $3, $4, $5}'
wc -c *.[^i][^a^\.][^a] | while read a b;
do
	if [ "$b" != "razem" ]; then
		size=`python3 <<< "import math; print(math.ceil($a/512))"`
		if [ $((begin+size)) -ge 36 ]; then							#may be 37, but it makes me feel safe
			begin=0
			track=$((track+1))
		fi
		printf "$b $size $a $track $begin" | awk '{ printf "%-10s %-10s %-10s %-10s %-10s\n", $1, $2, $3, $4, $5}'
		fill=`python3 <<< "print(' '*$((15-${#b})))"`
		echo "db \"$b\",0,\"$fill\", $track, $((begin+1)), $size" >> ../fat.asm

		dd if=$b of=disk.img seek=$((begin+(track-1)*72)) 2> /dev/null # 72*512=24204=0x9000 = new track
		dd if=/dev/zero of=disk.img seek=150 count=1	2> /dev/null
		begin=$((begin+size))
	fi
done
echo "db 0" >> ../fat.asm
echo "times 512-(\$-\$\$) db 0" >> ../fat.asm
cd ../../