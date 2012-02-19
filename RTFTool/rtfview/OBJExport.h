#ifndef __OBJEXPORT_H__
#define __OBJEXPORT_H__

#include "common.h"
void OBJExport();
void OBJUpdate(IMD_VERT *inVert);
void OBJEndStrip();
void OBJBegin(char *outfn);
void OBJSetTexture(GLuint texid);
void OBJAddMat(char *fn, GLuint id);

#endif // __OBJEXPORT_H__