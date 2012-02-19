/*  linked_lists.h

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

#ifndef __LINKED_LISTS_H__
#define __LINKED_LISTS_H__

#include "common.h"

void resetSegBase();
void pushSeg(IMD_SEG *seg);
IMD_SEG *nextSeg();
IMD_SEG *getSeg();
void deleteSegList();

void resetColVListBase();
void setColVListBase(IMD_COL_VERT_LIST *vList);
void pushColVList(IMD_COL_VERT_LIST *seg);
IMD_COL_VERT_LIST *nextColVList();
IMD_COL_VERT_LIST *getColVList();
void deleteColVList();

void resetVListBase();
void setVListBase(IMD_VERT_LIST *vList);
void pushVList(IMD_VERT_LIST *seg);
IMD_VERT_LIST *nextVList();
IMD_VERT_LIST *getVList();
void deleteVList();

void resetTexBase();
void pushTex(P6T_TEXTURE *tex);
P6T_TEXTURE *nextTex();
P6T_TEXTURE *getTex();
void popTex();
void deleteTexList();

void resetTexEnvBase();
void pushTexEnv(P6T_TEXTURE_ENV *tex);
P6T_TEXTURE_ENV *nextTexEnv();
P6T_TEXTURE_ENV *getTexEnv();
void popTexEnv();
void deleteTexEnvList();

#endif //__LINKED_LISTS_H__