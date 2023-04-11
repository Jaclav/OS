#!/bin/bash
cd bin/disk
begin=0

wc -c *.[^i][^m][^g^o] | while read a b;
do
	if [ "$b" != "razem" ]; then
		size=`python3 <<< "import math; print(math.ceil($a/512))"`
		# echo "Size: $size; begin: $begin" $b

		dd if=$b of=disk.img seek=$begin	2> /dev/null
		dd if=/dev/zero of=disk.img seek=100 count=1	2> /dev/null
		begin=$((begin+size))
	fi
done
cd ../../
dd if=bin/disk/disk.img of=bin/OS.img seek=72	2> /dev/null