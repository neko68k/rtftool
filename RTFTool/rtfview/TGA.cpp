#ifdef WIN32
	#include <windows.h>
#else
	#include <stdlib.h>
#endif
#include <stdio.h>
#include "common.h"
#include "TGA.h"

void TGA_Save(char *filename, unsigned char *pitopels, DWORD w, DWORD h)
{
	BYTE *newPitopels = (BYTE*)malloc(sizeof(pitopels));
	TGAImageHeader header;

	BYTE tetopture_bd = 2;

	header.id = 0;
	header.colormap = 0;
	if ( tetopture_bd == 1 )
		header.imageType = 3; // b+w image
	else{
		header.imageType = 2; // true colour image
		memset( header.colormapSpec, 0, sizeof( BYTE )*5 );
	}

	header.topOrigin = 0;
	header.yOrigin = 0;
	header.width = w;
	header.height = h;
	header.bitDepth = 32;
	header.imageDescription = 0;

	FILE *file = fopen( filename, "wb" );

	fwrite( &header, sizeof(TGAImageHeader), 1, file );
	fwrite( pitopels, sizeof(BYTE)*w*h*4, 1, file );	

	fclose(file);
}

	
