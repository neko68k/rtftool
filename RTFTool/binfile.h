#ifndef __BINFILE_H__
#define __BINFILE_H__

#include <stdio.h>

typedef struct
{
	DWORD size;
	BYTE *data;
	BYTE *base;
	BYTE *current;
	char *name;
}BINHANDLE;

#define BIN_SET 0
#define BIN_CUR 1
#define BIN_END 2

BINHANDLE *binopen(char* fn);
BINHANDLE *binopen(void *data, DWORD size);
void binclose(BINHANDLE *handle);
void binseek(BINHANDLE *handle, DWORD offset, BYTE origin);
void binread(void *buffer, size_t size, size_t count, BINHANDLE *handle);
DWORD bintell(BINHANDLE *handle);
#endif //__BINFILE_H__