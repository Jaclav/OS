/**
 * @file fs.h
 * @brief File system only for kernel use, users should use file.h
 * @todo better handle disk errors
 */
#ifndef FS_H
#define FS_H

#ifndef KERNEL
#error Only kernel should use this file!
#endif

#include <types.h>
#include <errno.h>
#include <io.h>
#include <string.h>
#include "interrupts.h"

#ifndef KERNEL_ADDRESS
#define KERNEL_ADDRESS 0
#error KERNEL_ADDRESS undefined!
#endif

#define FILENAME_MAX 16
#define SECTORS_PER_TRACK 37
#define TRACKS_MAX 3

//do it static - only this file should be able to use this
struct {
	char name[FILENAME_MAX];
	Byte track;
	Byte beginSector;
	Byte size;//! in sectors sector=512B
} *files;

bool map[TRACKS_MAX][SECTORS_PER_TRACK];//map[0] = 0 because there is no 0th sector, they are counted from 1

size_t numberOfFiles = 0;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"

static void saveFATable() {
	asm("push es\n"
	    "mov es, si\n"
	    "int 0x13\n"
	    "pop es"
	    ::"a"(0x0301), "d"(0), "c"(2), "b"(0), "S"(KERNEL_ADDRESS));
}

/**
 * @brief Copy from ss:source to cs:destination
 */
void __strncpy ( char * destination, const char * source, size_t num ) {
	asm("L%=:\n"
	    "	mov al, byte ptr ss:[si+bx]\n"
	    "	mov byte ptr cs:[di+bx], al\n"
	    "	cmp al,0\n"
	    "	je after%=\n"
	    "	inc bx\n"
	    "loop L%=\n"
	    "after%=:"
	    ::"D"(destination), "S"(source), "b"(0), "c"(num));
}

/**
 * @brief load file system
 *
 * @return int number of loaded files
 */
int sys_setup() {
	numberOfFiles = 0;
	if(*(Byte *)0 != 0xcf || *(Byte *)1 != 0xaa || *(Byte *)2 != 0x55)
		return -1;
	files = 3;
	for(; (*(int *)(numberOfFiles * 19 + 3) != 0) && numberOfFiles * 19 + 3 < 512; numberOfFiles++)
		for(int i = 0; i < files[numberOfFiles].size; i++) {
			if(files[numberOfFiles].track < TRACKS_MAX)
				map[files[numberOfFiles].track][files[numberOfFiles].beginSector + i] = 1;//reserve space
		}

	return numberOfFiles;
}

/**
 * @brief Open file
 *
 * @param filename
 * @return int {size,id} or -error
 */
int sys_open(const int filename) {
	for(size_t id = 0; id < numberOfFiles; id++) {
		if(strcmp(files[id].name, filename)) {
			return files[id].size << 8 | id;
		}
	}
	//file doesn't exist
	return -ENOENT;
}

/**
 * @brief Create file
 *
 * @param filename name
 * @param size in sectors(512B)
 * @return int new file's id or -error
 * @todo file must be all in one track, change it
 */
int sys_create(const int filename, size_t size) {
	//TODO: move it to sys_open when file wasn't found
	if(size <= 0)//must be at least 1 sector
		size = 1;

	if(size > SECTORS_PER_TRACK)
		return -EFBIG;

	if(sys_open(filename) != -ENOENT)
		return -EEXIST;

	//search for free space
	Byte beginSector = 0;//store first sector of unused cx sectors
	Byte track = 0;
	for(track = 0; track < TRACKS_MAX; track++) {
		for(int i = 1; i < SECTORS_PER_TRACK; i++) {
			beginSector = i;
			size_t j;
			for(j = 0; j < size && j + i < SECTORS_PER_TRACK; j++) {
				if(map[track][j + i]) {
					beginSector = 0;
					break;
				}
			}
			if(j < size)
				beginSector = 0;
			if(beginSector != 0)
				goto end;
		}
	}
end:
	if(beginSector == 0)
		return -ENOSPC;
	//set file's sectors as used
	for(size_t i = beginSector; i < size + beginSector; i++)
		map[track][i] = 1;

	files[numberOfFiles].beginSector = beginSector;
	files[numberOfFiles].size = size;
	files[numberOfFiles].track = track;
	strncpy(files[numberOfFiles].name, filename, FILENAME_MAX);
	numberOfFiles++;
	saveFATable();
	return numberOfFiles - 1;//return file's ID
}

/**
 * @brief Remove file
 *
 * @param filename
 * @return int 0 if success or -error
 */
int sys_remove(const int filename) {
	int data = sys_open(filename);
	if(data == -ENOENT)
		return -ENOENT;

	int id = (Byte)data;
	int track = files[id].track;

	//free space;
	for(int j = files[id].beginSector; j < files[id].beginSector + files[id].size; j++)
		map[files[id].track][j] = 0;

	//concat file table
	for(int i = id; files[i].track == track; i++) {
		files[i].beginSector = files[i + 1].beginSector;
		files[i].size = files[i + 1].size;
		files[i].track = files[i + 1].track;
		strcpy(files[i].name, files[i + 1].name);
	}
	numberOfFiles--;
	saveFATable();
	return 0;
}

/**
 * @brief Read from file
 *
 * @param id file's id
 * @param ptr pointer to destinated memory
 * @param size to read in bytes
 * @return int actually readed Bytes
 */
int sys_read(const Byte id, int ptr, size_t size) {
	if(id > numberOfFiles)
		return -ENOENT;

	int sectors = (size - (size % 512)) / 512;
	//if wanted more than filesize read whole file
	if(files[id].size < (size % 512 == 0 ? sectors : sectors + 1)) {
		size = files[id].size * 512;
		sectors = files[id].size;
	}

	//read whole sectors
	if(sectors > 0) {
		asm("int 0x13\n"
		    ::"a"(0x0200|sectors), "b"(ptr), "c"(files[id].track<<8|files[id].beginSector), "d"(0));
	}
	//read rest
	if(size % 512 != 0) {
		Byte readed[512];

		asm("push es\n"
		    "mov es, si\n"
		    "int 0x13\n"
		    "pop es\n"
		    ::"a"(0x0201), "b"(readed), "c"((files[id].track<<8|files[id].beginSector)+sectors), "d"(0), "S"(KERNEL_ADDRESS));

		//copy from cs:readed to ss:ptr
		asm("L%=:\n"
		    "	mov al, byte ptr cs:[si+bx]\n"
		    "	mov byte ptr ss:[di+bx], al\n"
		    "	inc bx\n"
		    "loop L%="
		    ::"D"(ptr+(sectors*512)), "S"(readed), "b"(0), "c"(size%512));
	}
	return size;
}

/**
 * @brief Write to file
 *
 * @param id file's id
 * @param ptr pointer to source memory
 * @param size to write in Bytes, file will be rounded to whole sectors
 * @return int actually writted Bytes
 * @todo add modes, when append when override
 */
int sys_write(Byte id, int ptr, size_t size) {
	if(id > numberOfFiles)
		return -ENOENT;

	int sectors = (size - (size % 512)) / 512;
	//if not enough space try resizing
	if(files[id].size < sectors + (size % 512 != 0 ? 1 : 0)) {
		char filename[FILENAME_MAX];
		strcpy(filename, files[id].name);
		int ret = sys_remove(filename);
		if(ret < 0)return ret;

		ret = sys_create(filename, sectors + (size % 512 != 0 ? 1 : 0));
		if(ret < 0)return ret;
		id = ret;
	}

	//save whole sectors
	if(sectors > 0) {
		asm("int 0x13"
		    ::"a"(0x0300|sectors), "b"(ptr), "c"(files[id].track<<8|files[id].beginSector), "d"(0));
	}
	//save rest
	if(size % 512 != 0) {
		Byte toSave[512];
		memset(toSave, 0, 512);

		//copy from ss:ptr to cs:toSave
		asm("L%=:\n"
		    "	mov al, byte ptr ss:[si+bx]\n"
		    "	mov byte ptr cs:[di+bx], al\n"
		    "	inc bx\n"
		    "loop L%="
		    ::"D"(toSave), "S"(ptr + (size - (size % 512))), "b"(0), "c"(size % 512));

		asm("push es\n"
		    "mov es, si\n"
		    "int 0x13\n"
		    "pop es"
		    ::"a"(0x0301), "b"(toSave), "c"(files[id].track<<8|(files[id].beginSector+sectors)), "d"(0), "S"(KERNEL_ADDRESS));
	}
	return size;
}

/**
 * @brief interruption for file system
 * @details sets all sys_* functions as interruption
 * @param registers AX, BX, CX, DX, SI, DI
 * @return values are in registers AX, BX
 */
__int void int0x21(interruptFrame * frame) {
	//see interrupts.asm
	//TODO change order of ah
	//DS and CS are on kernel address
	asm("push ds\nmov ds, ax"::"a"(KERNEL_ADDRESS));

	// registers are backed up, after this function, they are restored
	int ax = 0, bx = 0, cx = 0, dx = 0, di = 0, si = 0;
	asm("mov ax,[ebp-24]":"=a"(ax));
	asm("mov dx,[ebp-20]":"=d"(dx));
	asm("mov cx,[ebp-16]":"=c"(cx));
	asm("mov bx,[ebp-12]":"=b"(bx));
	asm("mov si,[ebp-8]":"=S"(si));
	asm("mov di,[ebp-4]":"=D"(di));
	//change ah to ax
	char fileName[FILENAME_MAX];
	switch(ax >> 8) {
	case 0:
		asm("mov [ebp-24], eax"::"a"(sys_setup()));
		break;
	case 1:
		__strncpy(fileName, bx, FILENAME_MAX);
		asm("mov [ebp-24], eax"::"a"(sys_open(fileName)));
		break;
	case 2:
		asm("mov [ebp-24], eax"::"a"(sys_read(bx, cx, dx)));
		break;
	case 3:
		asm("mov [ebp-24], eax"::"a"(sys_write(bx, cx, dx)));
		break;
	case 4:
		__strncpy(fileName, bx, FILENAME_MAX);
		asm("mov [ebp-24], eax"::"a"(sys_create(fileName, cx)));
		break;
	case 5:
		__strncpy(fileName, bx, FILENAME_MAX);
		asm("mov [ebp-24], eax"::"a"(sys_remove(fileName)));
		break;
	default:
		printf("INT 0x21!\n");
		asm("mov [ebp-24],eax"::"a"(ENOSYS));
		break;
	}
	asm("pop ds");
}
#pragma GCC diagnostic pop

#endif