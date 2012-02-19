#ifndef __TGA_H__
#define __TGA_H__

#ifdef WIN32
	#include <windows.h>
#endif

typedef struct
{
// imagetype 2==truecolour uncompressed,
// 3==b+w uncompressed (theres no implementational difference between the two)

BYTE id; // the number of bytes in image ID (comes after imageDescription) + before the actual image data
BYTE colormap;
BYTE imageType;
BYTE colormapSpec[5];

WORD topOrigin;
WORD yOrigin; 
WORD width;
WORD height;
BYTE bitDepth;

BYTE imageDescription;
} TGAImageHeader;

void TGA_Save(char *filename, unsigned char *pitopels, DWORD w, DWORD h);
////////////////////////////

#endif //__TGA_H__
