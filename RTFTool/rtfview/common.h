/*  common.h

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

#ifndef __COMMON_H__
#define __COMMON_H__

#include <Windows.h>
#include <GL/gl.h>

//#define DWORD unsigned int
//#define WORD unsigned short
//#define BYTE unsigned char
#ifndef WIN32
	#define MAX_PATH 260
#endif
#define FALSE_COLOR 0
#define dump 0


typedef struct
{
	BYTE r;
	BYTE g;
	BYTE b;
	BYTE a;
}RTF_COLOR;

typedef struct
{
	DWORD offset;
}IMD_OFS;

typedef struct
{
	char magic[4];
	WORD unk1;
	WORD pad1;
	DWORD pad2;
	
	WORD unk2;
	WORD pad[9];
	
	WORD typeOffset;
	WORD unk3[7];
}IMD_HEAD1;

typedef struct
{
	DWORD unk;
	WORD unk2;
	WORD unk3;	
	WORD totalVerts;	// including 0x00000017 headers (12*sizeof(dword)) each
	BYTE unk4[0xE];
}IMD_VERTINFO;		// 0x40 block header

typedef struct
{
	BYTE unk1[0xC];
	DWORD meshColor;
	BYTE numVerts;
	BYTE unk2[0xF];
	BYTE numVerts2;
	BYTE unk3[0xF];
}IMD_VERTSUBINFO;

typedef struct
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	DWORD flag;

	GLfloat normalx;
	GLfloat normaly;
	GLfloat normalz;
	DWORD normalflag;

	GLfloat texcoordx;
	GLfloat texcoordy;
	GLfloat texcoordz;
	DWORD texcoordflag;


}IMD_VERT;

typedef struct
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	DWORD flag;
}IMD_SIMPLE_VERT;

typedef struct
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	DWORD flag;

	GLfloat normalx;
	GLfloat normaly;
	GLfloat normalz;
	DWORD normalflag;

	GLfloat texcoordx;
	GLfloat texcoordy;
	GLfloat texcoordz;
	DWORD texcoordflag;

	GLfloat R;
	GLfloat G;
	GLfloat B;
	GLfloat A;
}IMD_COL_VERT;

typedef struct
{	
	GLuint texID;
	DWORD width;
	DWORD height;
	DWORD unk;
	RTF_COLOR color1;
	RTF_COLOR color2;
	RTF_COLOR color3;
	RTF_COLOR color4;
	BYTE unk99[0x30];

	BYTE *pixels;
	GLuint texFN;	
	void *next;
}P6T_TEXTURE;

typedef struct
{
	BYTE *pixels;
	RECT *rect;
}
P6T_EXPORT;

typedef struct
{	
	GLuint texID;
	DWORD width;
	DWORD height;
	DWORD unk;

	GLuint texFN;	
	void *next;
}P6T_TEXTURE_ENV;

typedef struct
{
	GLfloat vertColor[4];
	GLfloat unk1[4];
	GLfloat unk2[4];
	DWORD unk3[4];
}P6T_TEXTURE_MAT;

typedef struct
{
	IMD_VERTINFO *header;
	IMD_COL_VERT *verts;
	GLuint texID;	
	DWORD realNumVerts;
	void *next;
}IMD_COL_VERT_LIST;

typedef struct
{
	IMD_VERTINFO *header;
	IMD_VERT *verts;
	GLuint texID;	
	DWORD realNumVerts;
	DWORD meshColor;

	void *next;
}IMD_VERT_LIST;

typedef struct
{
	IMD_SIMPLE_VERT verts[5];
	IMD_SIMPLE_VERT unk1;
}IMD_LINK;


typedef struct
{
	IMD_LINK link;
	IMD_SIMPLE_VERT hitbox[8];
	IMD_VERT_LIST *vertLists;	// linked list, use with setVListBase(IMD_VERT_LIST *)
	IMD_COL_VERT_LIST *vertColLists;
	void *next;
}IMD_SEG;		// 0x01 block and children

// tag at offset 0x30
#define IMD_TYPE_MESH			0x10	// mesh type IMD
#define IMD_TYPE_ANIM			0x20	// animation type IMD
#define IMD_TYPE_LIGHT			0x31	// light type IMD

// data tables
#define IMD_BLOCK_SEG			0x01	// seg
#define IMD_BLOCK_LINK_TBL		0x02	// link data table for indexed reference, present if numLinks > 1

// system related info
#define IMD_BLOCK_LINKINFO		0x10	// link data for relative reference
#define IMD_BLOCK_HITBOX		0x13	// relative hitbox

// the 0x20 group is probably all materials and texture related
#define IMD_BLOCK_TEXINFO_MAT		0x20	// first 4 floats are the mesh color, 4 sets of unsigned byte color?
#define IMD_BLOCK_TEXINFO			0x21	// texture info
#define IMD_BLOCK_TAG_22			0x22	// unk
#define IMD_BLOCK_TEXINFO23			0x23	// Identical to TEXINFO but paired with an 0x25
#define IMD_BLOCK_TAG_24			0x24	// unk
#define IMD_BLOCK_TAG_25			0x25	// unk, grouped with an 0x23
#define IMD_BLOCK_TAG_26			0x26	// unk
#define IMD_BLOCK_TEXINFO_ENV		0x28	// simple, only refers to filename and W/H, some kind of env mapping texture
										// seen used in conjunction with the canopy

// 0x40 group is primitive related
#define IMD_BLOCK_VERTINFO		0x40	// vertex info
#define IMD_BLOCK_VERTINFO2		0x41	// colored vertex w/ texture for fakelighting
#define IMD_BLOCK_VERTINFO3		0x42	// same layout as VERTINFO with tex coords set to 0. used for env mapping

#define IMD_BLOCK_TAG_50		0x50	//unknown
#define IMD_BLOCK_TAG_52		0x52	//unknown

#define IMD_BLOCK_TAG_60		0x60	//unknown, stage movement and camera pathing datas?
#define IMD_BLOCK_TAG_62		0x62	//unknown
#define IMD_BLOCK_TAG_68		0x68	//unknown

#define IMD_BLOCK_TAG_70		0x70	// anim related
#define IMD_BLOCK_TAG_71		0x71	// anim related

#define IMD_BLOCK_TAG_B0		0xB0	// lighting related
#define IMD_BLOCK_TAG_B1		0xB1	// lighting related




#endif // __COMMON_H__
