#include <windows.h>
#include "icon_common.h"
#include <gl/GL.h>

typedef struct
{
	DWORD id;
	DWORD numShapes;
	DWORD texType;	// 0x07 = uncompressed, else RLE
	DWORD unk;		// always 0x3F800000
	DWORD numVerts; // always multiple of 3
}ICONHDR;

typedef struct
{
	short x;
	short y;
	short z;
	WORD lightingFlag;
}VERTEX;

typedef struct
{
	short x;
	short y;
	BYTE rgba[4];
}TEXCOORD;

static VERTEX *verts = NULL;
static VERTEX *normals = NULL;
static TEXCOORD *texCoords = NULL;
static ICONHDR *iconHeader = NULL;	

void ICON_Load(BYTE *inData)
{
	
	
	
}