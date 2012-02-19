/*  linked_lists.cpp

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
#include "linked_lists.h"
#include "common.h"
#pragma unmanaged
static IMD_SEG *segStart = NULL;
static IMD_SEG *segEnd = NULL;
static IMD_SEG *segCurrent = NULL;

// regular verts
static IMD_VERT_LIST *vlStart = NULL;
static IMD_VERT_LIST *vlEnd = NULL;
static IMD_VERT_LIST *vlCurrent = NULL;

// colored verts
static IMD_COL_VERT_LIST *vlColStart = NULL;
static IMD_COL_VERT_LIST *vlColEnd = NULL;
static IMD_COL_VERT_LIST *vlColCurrent = NULL;

static P6T_TEXTURE *texStart = NULL;
static P6T_TEXTURE *texEnd = NULL;
static P6T_TEXTURE *texCurrent = NULL;
static P6T_TEXTURE *texPrev = NULL;

static P6T_TEXTURE_ENV *texEnvStart = NULL;
static P6T_TEXTURE_ENV *texEnvEnd = NULL;
static P6T_TEXTURE_ENV *texEnvCurrent = NULL;
static P6T_TEXTURE_ENV *texEnvPrev = NULL;
#pragma managed

/***********************************/
/*       LINKED LIST CLEANUP       */
/***********************************/
void deleteSegList()
{
	IMD_SEG *next = NULL;
	IMD_SEG *prev = NULL;
	if(segStart == NULL)
		return;

	next = segStart;
	prev = segStart;
	while(next!=NULL)
	{
		vlStart = next->vertLists;
		deleteVList();
		next->vertLists = NULL;
		vlColStart = next->vertColLists;
		deleteColVList();
		next->vertColLists = NULL;
		prev = next;
		next = (IMD_SEG*)prev->next;
		free(prev);
		prev = NULL;
	}
	segStart = NULL;
}

void deleteColVList()
{
	IMD_COL_VERT_LIST *next = NULL;
	IMD_COL_VERT_LIST *prev = NULL;
	if(vlColStart == NULL)
		return;

	next = vlColStart;
	prev = vlColStart;
	while(next!=NULL)
	{		
		free(next->header);
		next->header = NULL;
		free(next->verts);
		next->verts = NULL;
		prev = next;
		next = (IMD_COL_VERT_LIST*)prev->next;
		free(prev);
		prev = NULL;
	}
	vlColStart = NULL;
}

void deleteVList()
{
	IMD_VERT_LIST *next = NULL;
	IMD_VERT_LIST *prev = NULL;
	if(vlStart == NULL)
		return;

	next = vlStart;
	prev = vlStart;
	while(next!=NULL)
	{		
		free(next->header);
		next->header = NULL;
		free(next->verts);
		next->verts = NULL;
		prev = next;
		next = (IMD_VERT_LIST*)prev->next;
		free(prev);
		prev = NULL;
	}
	vlStart = NULL;
}

void deleteTexEnvList()
{
	P6T_TEXTURE_ENV *next = NULL;
	P6T_TEXTURE_ENV *prev = NULL;
	if(texEnvStart == NULL)
		return;

	next = texEnvStart;
	prev = texEnvStart;
	while(next!=NULL)
	{		
		prev = next;
		next = (P6T_TEXTURE_ENV*)prev->next;
		free(prev);
		prev = NULL;
	}
	texEnvStart = NULL;
}

void deleteTexList()
{
	P6T_TEXTURE *next = NULL;
	P6T_TEXTURE *prev = NULL;
	if(texStart == NULL)
		return;

	next = texStart;
	prev = texStart;
	while(next!=NULL)
	{		
		free(next->pixels);
		next->pixels = NULL;
		prev = next;
		next = (P6T_TEXTURE*)prev->next;
		free(prev);
		prev = NULL;
	}
	texStart = NULL;
}

/***********************/
/*     END CLEANUP     */
/***********************/

void resetSegBase()
{
	segCurrent = segStart;
}

void pushSeg(IMD_SEG *seg)
{
	if(segStart == NULL)
	{
		segStart = seg;
		segStart->next = NULL;
		segEnd = segStart;
		segCurrent = segStart;
	}
	else
	{
		segEnd->next = seg;
		segEnd = seg;
		segEnd->next = NULL;
	}
}

IMD_SEG *nextSeg()
{
	if(segCurrent->next != NULL)
	{
		segCurrent = (IMD_SEG*)segCurrent->next;
		return(segCurrent);
	}
	else
	{
		segCurrent = segStart;
		return NULL;
	}
}

IMD_SEG *getSeg()
{
	return segCurrent;
}

/////////////////////////////////////////////////////////////
void resetColVListBase()
{
	vlColCurrent = vlColStart;
	//return;
}
void setColVListBase(IMD_COL_VERT_LIST *vList)
{	
		vlColStart = vList;
		vlColCurrent = vlColStart;
		vlColEnd = vlColCurrent;
}

void pushColVList(IMD_COL_VERT_LIST *seg)
{
	if(vlStart == NULL)
	{
		vlColStart = seg;
		vlColStart->next = NULL;
		vlColEnd = vlColStart;
		vlColCurrent = vlColStart;
	}
	else
	{
		vlColEnd->next = seg;
		vlColEnd = seg;
		vlColEnd->next = NULL;
	}
}

IMD_COL_VERT_LIST *nextColVList()
{
	if(vlColCurrent->next != NULL)
	{
		vlColCurrent = (IMD_COL_VERT_LIST*)vlColCurrent->next;
		return(vlColCurrent);
	}
	else
	{
		vlColCurrent = vlColStart;
		return NULL;
	}
}

IMD_COL_VERT_LIST *getColVList()
{
	return vlColCurrent;
}

/////////////////////////////////////////////////////////////
void resetVListBase()
{
	vlCurrent = vlStart;
	//return;
}
void setVListBase(IMD_VERT_LIST *vList)
{	
		vlStart = vList;
		vlCurrent = vlStart;
		vlEnd = vlCurrent;
}

void pushVList(IMD_VERT_LIST *seg)
{
	if(vlStart == NULL)
	{
		vlStart = seg;
		vlStart->next = NULL;
		vlEnd = vlStart;
		vlCurrent = vlStart;
	}
	else
	{
		vlEnd->next = seg;
		vlEnd = seg;
		vlEnd->next = NULL;
	}
}

IMD_VERT_LIST *nextVList()
{
	if(vlCurrent->next != NULL)
	{
		vlCurrent = (IMD_VERT_LIST*)vlCurrent->next;
		return(vlCurrent);
	}
	else
	{
		vlCurrent = vlStart;
		return NULL;
	}
}

IMD_VERT_LIST *getVList()
{
	return vlCurrent;
}

/////////////////////////////////////////////////////////////
void resetTexBase()
{
	texCurrent = texStart;
}
void pushTex(P6T_TEXTURE *tex)
{
	if(texStart == NULL)
	{
		texStart = tex;
		texStart->next = NULL;
		texEnd = texStart;
		texCurrent = texStart;
		texPrev = NULL;
	}
	else
	{
		texEnd->next = tex;
		texPrev = texEnd;
		texEnd = tex;
		texEnd->next = NULL;
	}
}

P6T_TEXTURE *nextTex()
{
	if(texCurrent!=NULL)
	{
		if(texCurrent->next != NULL)
		{
			texPrev = texCurrent;
			texCurrent = (P6T_TEXTURE*)texCurrent->next;
			return(texCurrent);
		}
		else
		{
			texCurrent = texStart;
			texPrev = NULL;
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}

P6T_TEXTURE *getTex()
{	
	return texCurrent;
}

void popTex()
{
	P6T_TEXTURE *temp = NULL;

	if(texCurrent->next != NULL)
	{
		temp = texCurrent;
		if(texPrev != NULL)
		{
			texPrev->next = temp->next;
		}
		texCurrent = texPrev;
		free(temp);
	}
}

/////////////////////////////////////////////////////////////
void resetTexEnvBase()
{
	texEnvCurrent = texEnvStart;
}
void pushTexEnv(P6T_TEXTURE_ENV *tex)
{
	if(texEnvStart == NULL)
	{
		texEnvStart = tex;
		texEnvStart->next = NULL;
		texEnvEnd = texEnvStart;
		texEnvCurrent = texEnvStart;
		texEnvPrev = NULL;
	}
	else
	{
		texEnvEnd->next = tex;
		texEnvPrev = texEnvEnd;
		texEnvEnd = tex;
		texEnvEnd->next = NULL;
	}
}

P6T_TEXTURE_ENV *nextTexEnv()
{
	if(texEnvCurrent!=NULL)
	{
		if(texEnvCurrent->next != NULL)
		{
			texEnvPrev = texEnvCurrent;
			texEnvCurrent = (P6T_TEXTURE_ENV*)texCurrent->next;
			return(texEnvCurrent);
		}
		else
		{
			texEnvCurrent = texEnvStart;
			texEnvPrev = NULL;
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}

P6T_TEXTURE_ENV *getTexEnv()
{	
	return texEnvCurrent;
}

void popTexEnv()
{
	P6T_TEXTURE_ENV *temp = NULL;

	if(texEnvCurrent->next != NULL)
	{
		temp = texEnvCurrent;
		if(texEnvPrev != NULL)
		{
			texEnvPrev->next = temp->next;
		}
		texEnvCurrent = texEnvPrev;
		free(temp);
	}
}



