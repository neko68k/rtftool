#include <Windows.h>
#include "binfile.h"

BINHANDLE *binopen(char* fn)
{

	DWORD size;
	FILE *inFile = fopen(fn, "rb");
	BINHANDLE *handle = (BINHANDLE*)malloc(sizeof(BINHANDLE));
	fseek(inFile, 0, SEEK_END);
	size = ftell(inFile);
	fseek(inFile, 0, SEEK_SET);
	handle->data = handle->current = (BYTE*)malloc(size);
	handle->size = size;
	fread(handle->data, size, 1, inFile);
	fclose(inFile);
	return(handle);
}
BINHANDLE *binopen(void *data, DWORD size)
{
	BINHANDLE *handle = (BINHANDLE*)calloc(1, sizeof(BINHANDLE));
	handle->size = size;
	handle->base = handle->current = (BYTE*)data;
	return(handle);
}
void binclose(BINHANDLE *handle)
{
	if(handle)
	{
		if(handle->base)
			free(handle->base);
		free(handle);
	}
	handle = NULL;
}
void binseek(BINHANDLE *handle, DWORD offset, BYTE origin)
{
	switch(origin)
	{
	case BIN_SET:
		handle->current = handle->base + offset;
		break;
	case BIN_END:
		handle->current = handle->base+handle->size+offset;
		break;
	case BIN_CUR:
		handle->current = handle->current+offset;
		break;
	default:
		break;
	};
}
void binread(void *buffer, size_t size, size_t count, BINHANDLE *handle)
{
	memcpy(buffer, handle->current, size*count);
	handle->current = handle->current + (size*count);
}

DWORD bintell(BINHANDLE *handle)
{
	return(handle->current-handle->base);

}
