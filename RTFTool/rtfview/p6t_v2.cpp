/*  p6t.cpp

    Copyright 2010 Shaun Thompson

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#include <windows.h>
#include <stdio.h>
#include <direct.h>

#include "common.h"

#define LONIBBLE(x)  ((BYTE) ((BYTE) (x) & (BYTE) 0x0F))
#define HINIBBLE(x)  ((BYTE) ((BYTE) (x) >> (BYTE) 4))

#define P6T_32BIT				0x04
#define P6T_8BIT				0x01
#define P6T_4BIT				0x00


typedef struct
{
	char magic1[4];
	DWORD magic2;
	
	WORD w;
	WORD pad1;

	WORD h;
	WORD pad2;

	WORD palOffset;
	WORD pad3;
	
	WORD pixelOffset;
	WORD pad4;

	WORD unk1;
	WORD pad5;
	
	WORD imgType;	// see defines in common.h
	WORD pad6;
	DWORD pad7;
	
	WORD swizFlag; 
	DWORD pad8;
	
	WORD unk3;

}P6T_HEAD;

P6T_EXPORT P6T_Load(BYTE *data, GLuint texID, char *exePath, BYTE stat, bool flip);

void P6T_ProcessImg4(P6T_HEAD *header, BYTE *inpixels, BYTE *outpixels)
{
	int x, y;
	int w = header->w;
	int h = header->h;	
	x=y=0;
	BYTE *fixPixels = (BYTE*)malloc(w*h);
	BYTE entry;
	//FILE *bindump;

	if(header->swizFlag != 1)
	{
		for(x=0,y=0;y<header->h*header->w;x++,y+=2)
		{
			outpixels[y] = LONIBBLE(inpixels[x]);
			outpixels[y+1] = HINIBBLE(inpixels[x]);
		}	
		
		return;
	}
	
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			// get the pen
 			int index = (y * w) + x ;
			
 
			// swizzle
 			int pageX = x &(~0x7f);
			int pageY = y &(~0x7f);

			int pages_horz = (w + 127) / 128;
			int pages_vert = (h + 127) / 128;
 
			int page_number = (pageY / 128) * pages_horz + (pageX / 128);
 
 			int page32Y = (page_number / pages_vert) * 32;
 			int page32X = (page_number % pages_vert) * 64;
 
			int page_location = page32Y * h * 2 + page32X * 4;

			int locX = x & 0x7f;
			int locY = y & 0x7f;
 
			int block_location = ((locX & (~0x1f)) >> 1) * h + (locY & (~0xf)) * 2;
			int swap_selector = (((y + 2) >> 2) & 0x1) * 4;
 			int posY = (((y & (~3)) >> 1) + (y & 1)) & 0x7;

 			int column_location = posY * h * 2 + ((x + swap_selector) & 0x7) * 4;
 
   			int byte_num = (x >> 3) & 3;     // 0,1,2,3
 			
			entry = inpixels[page_location + block_location + column_location + byte_num];
			entry = (BYTE)((entry >> ((y >> 1) & 0x01) * 4) & 0x0F);
			outpixels[index] = entry;
		}
	}
}

void P6T_ProcessImg8(P6T_HEAD *header, BYTE *inpixels, BYTE *outpixels)
{
	int i = 0;

	if(header->swizFlag != 1)
	{
		memcpy(outpixels, inpixels, header->w*header->h);
		return;
	}

    for (int y = 0; y < header->h; y++)
    {
        for (int x = 0; x < header->w; x++)
        {
            int block_location = (y & (~0xf)) * header->w + (x & (~0xf)) * 2;
            int swap_selector = (((y + 2) >> 2) & 0x1) * 4;
            int posY = (((y & (~3)) >> 1) + (y & 1)) & 0x7;
            int column_location = posY * header->w * 2 + ((x + swap_selector) & 0x7) * 4;

            int byte_num = ((y >> 1) & 1) + ((x >> 2) & 2);     // 0,1,2,3

            outpixels[(y * header->w) + x] = inpixels[block_location + column_location + byte_num];
        }
    }

}

void P6T_8to32(P6T_HEAD *header, RTF_COLOR *CLUT, BYTE *inPixels, RTF_COLOR *outPixels)
{
	DWORD i = 0;
	for(i = 0; i < header->w*header->h; i++)
	{
		outPixels[i] = CLUT[inPixels[i]];
		if(outPixels[i].a != 0x00)
			outPixels[i].a = (outPixels[i].a << 1)-1;		
	}
}

void P6T_4to32(P6T_HEAD *header, RTF_COLOR *CLUT, BYTE *inPixels, RTF_COLOR *outPixels)
{
	DWORD i = 0;
	
	for(i = 0; i < header->w*header->h; i++)
	{		
		if(LONIBBLE(inPixels[i]) == 0 && HINIBBLE(inPixels[i]) == 0)
			outPixels[i] = CLUT[0];
		else if(LONIBBLE(inPixels[i]) == 0 && HINIBBLE(inPixels[i]) != 0)
			outPixels[i] = CLUT[HINIBBLE(inPixels[i])];	
		else if(LONIBBLE(inPixels[i]) != 0 && HINIBBLE(inPixels[i]) == 0)
			outPixels[i] = CLUT[LONIBBLE(inPixels[i])];
		
		if(outPixels[i].a != 0x00)
			outPixels[i].a = (outPixels[i].a << 1)-1;
	}
}

void P6T_LoadCLUT(FILE *infile, RTF_COLOR *CLUT, DWORD numColors)
{
	DWORD i = 0;
	RTF_COLOR *locCLUT = CLUT;

	if(numColors>32)
	{
		for(i = 0; i<numColors; i+=32)
		{
			fread(&locCLUT[i], 4*8, 1, infile);
			fread(&locCLUT[i+16], 4*8, 1, infile);
			fread(&locCLUT[i+8], 4*8, 1, infile);
			fread(&locCLUT[i+24], 4*8, 1, infile);		
		}
	}
	else
	{
		fread(locCLUT, 32*4, 1, infile);
	}
}

void P6T_LoadCLUT(BYTE *data, RTF_COLOR *CLUT, DWORD numColors)
{
	DWORD i = 0;
	RTF_COLOR *locCLUT = CLUT;

	if(numColors>32)
	{
		for(i = 0; i<numColors; i+=32)
		{
			memcpy(&locCLUT[i], data, 4*8);
			data = data+(4*8);
			memcpy(&locCLUT[i+16], data, 4*8);
			data = data+(4*8);
			memcpy(&locCLUT[i+8], data, 4*8);
			data = data+(4*8);
			memcpy(&locCLUT[i+24], data, 4*8);
			data = data+(4*8);
		}
	}
	else
	{
		memcpy(locCLUT, data, 34*2);
	}
}

P6T_EXPORT P6T_LoadTexture(char *fn, GLuint texID, char *exePath, BYTE stat)
{
	FILE *inFile = NULL;
	BYTE *data = NULL;
	DWORD size = 0;
	FILE *configFile = NULL;
	FILE *infile = NULL;
	char fqp[MAX_PATH];	
	char locExePath[MAX_PATH];
	memset(fqp, 0, MAX_PATH);

	if(!stat)
	{
		configFile = fopen("rtfview.conf", "r");
		if(configFile == NULL)
		{
			sprintf(locExePath, "%s/rtfview.conf", exePath);
			configFile = fopen(locExePath, "r");
		}
		fgets(fqp, MAX_PATH, configFile);
		fclose(configFile);

		strcat(fqp, fn);
		printf("%s\n", fqp);
		inFile = fopen(fqp, "rb");
		
	}
	else
	{
		printf("%s\n", fn);
		inFile = fopen(fn, "rb");
	}
	
	fseek(inFile, 0, SEEK_END);
	size = ftell(inFile);
	fseek(inFile, 0, SEEK_SET);
	data = (BYTE*)malloc(size);
	fread(data, size, 1, inFile);
	fclose(inFile);
	return(P6T_Load(data, texID, exePath, stat, false));
}

P6T_EXPORT P6T_LoadTexture(BYTE *data, GLuint texID,  BYTE stat, bool flip)
{
	return(P6T_Load(data, texID, "", stat, flip));
}


P6T_EXPORT P6T_Load(BYTE *data, GLuint texID, char *exePath, BYTE stat, bool flip)
{	
	P6T_HEAD *header = NULL;
	P6T_EXPORT exportData;
	void *readPixels = NULL;
	void *outPixels = NULL;
	BYTE *flipPixels = NULL;
	BYTE *pixels = NULL;
	RTF_COLOR *CLUT = NULL;

	DWORD numColors = 0;
	DWORD numBytes = 0;
	DWORD storedBytes = 0;
	DWORD pixSize = 0;
	DWORD clutSize = 0;
	FILE *bindump = NULL;
	BYTE *workingData = NULL;
	
	exportData.rect = (RECT*)malloc(sizeof(RECT));
	exportData.rect->bottom = exportData.rect->right = exportData.rect->top = exportData.rect->bottom = 0;
	

	if(data == NULL)
	{
		printf("Requested P6T does not exist!\n");
		return exportData;
	}
	
	header = (P6T_HEAD*)data;

	exportData.rect->bottom = header->h;
	exportData.rect->right = header->w;
	exportData.rect->top = 0;
	exportData.rect->left = 0;

	if(!strcmp(header->magic1, "P6T*"))
	{
		printf("Requested file is not a valid P6T!\n");
		return exportData;
	}

	pixels = (BYTE*)calloc(sizeof(BYTE), header->w*header->h*sizeof(DWORD));

	switch(header->imgType)
	{
	case P6T_4BIT:
		storedBytes = (header->h*header->w)/2;
		outPixels = malloc((header->w)*(header->h));
		
		CLUT = (RTF_COLOR*)malloc(32*sizeof(RTF_COLOR));
		if(CLUT != NULL)
			memset(CLUT, 0, (32*sizeof(RTF_COLOR)));
		workingData = data+header->palOffset;
		P6T_LoadCLUT(workingData, CLUT, 32);
		readPixels = data+header->pixelOffset;
		P6T_ProcessImg4(header, (BYTE*)readPixels, (BYTE*)outPixels);
		P6T_4to32(header, CLUT, (BYTE*)outPixels, (RTF_COLOR*)pixels);

		break;

	case P6T_8BIT:
		
		storedBytes = header->w*header->h;
		outPixels = malloc(storedBytes);
		
		CLUT = (RTF_COLOR*)malloc(256*sizeof(RTF_COLOR));
		if(CLUT != NULL)
			memset(CLUT, 0, (256*sizeof(RTF_COLOR)));
		workingData = data+header->palOffset;
		P6T_LoadCLUT(workingData, CLUT, 256);
		readPixels = data+header->pixelOffset;
		P6T_ProcessImg8(header, (BYTE*)readPixels, (BYTE*)outPixels);
		P6T_8to32(header, CLUT, (BYTE*)outPixels, (RTF_COLOR*)pixels);
		break;
		
	case P6T_32BIT:
		storedBytes = (header->w*header->h)*sizeof(RTF_COLOR);
		readPixels = malloc(storedBytes);		
		readPixels = data + header->pixelOffset;
		memcpy(pixels, readPixels, storedBytes);			

		break;
		
	default:
		break;
	}

	if(texID == GL_INVALID_VALUE || texID == GL_INVALID_OPERATION)
		printf("GL: texture error\n");
 
	glBindTexture( GL_TEXTURE_2D, texID );
 
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
 

	exportData.pixels = (BYTE*)malloc(header->w*header->h*4);
	
	flipPixels = (BYTE*)malloc(header->w*header->h*4);
	int i = 0;
	int j = 0;
	// BGRA to RGBA
	for(i = 0 ; i < header->w*header->h*4; i+=4)
	{		
		memcpy(&flipPixels[i+2], (BYTE*)pixels+i, 1);
		memcpy(&flipPixels[i+1], (BYTE*)pixels+i+1, 1);
		memcpy(&flipPixels[i], (BYTE*)pixels+i+2, 1);
		memcpy(&flipPixels[i+3], (BYTE*)pixels+i+3, 1);
	}
	
	// flip vertically
	if(flip)
	{
		j=header->h-1;
		for(i = 0; i< header->h; i++, j--)
		{		
			memcpy(&exportData.pixels[j*header->w*4], &flipPixels[i*header->w*4], header->w*4);
		}
	}
	else
		exportData.pixels = flipPixels;

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, header->w, header->h, 0,
					  GL_RGBA, GL_UNSIGNED_BYTE, pixels );
		
return exportData;
}
