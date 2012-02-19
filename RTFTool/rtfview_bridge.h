#ifndef __RTFVIEW_BRIDGE_H__
#define __RTFVIEW_BRIDGE_H__
#include <Windows.h>
#include <gl/GL.h>

extern GLuint IMD_DrawBlocks(DWORD numSegs, BYTE exp);
DWORD IMD_Load(char *fn);
DWORD IMD_Load(BYTE *data, DWORD size, BYTE exp);

#endif