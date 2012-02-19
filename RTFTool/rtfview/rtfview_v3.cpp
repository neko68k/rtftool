/*  rtfview_v3.cpp

    Copyright 2009 Shaun Thompson

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

/***************************************************************************
0x01 blocks: 
the first one, which refers to a 0x02 block, which in turn refers to 0x10 
blocks. The remainder, for example:

0x10|0x13|0x20|0x21|0x40		// vertices
          0x20|0x21|0x40		// normals
          0x20|0x21|0x40		// texcoords

We must read each offset in the 0x01 and 0x02 blocks until we hit NULL and 
we have loaded all of the vertex blocks.

Other blocks known as follows:
0x02 - always points to 0x10 blocks
0x10 - unknown, contains floats, fixed length, 6 QWORDS (most likely material definitions)
0x13 - hit box, fixed length, 8 QWORDS
0x20 - unknown, fixed length, 4 QWORDS
0x21 - unknown, fixed length, 5 QWORDS (second QWORD, first WORD is texture file)
0x40 - vertex block, fixed length header, variable length data, 
	   vertCount-1 is 8th WORD past 0x4000

***************************************************************************/

#include <windows.h>
#include <direct.h>	
#include <GL/gl.h>
#include <math.h>
#include "common.h"
#include "linked_lists.h"
#include "p6t.h"
#include "imd.h"
#include "TGA.h"
#include "../binfile.h"
#include "DAT.h"
#include "OBJExport.h"

//SDL_Surface *surface;	// This surface will tell us the details of the image


GLfloat xrot, yrot, zrot;
GLfloat xtrans, ytrans, ztrans;
static GLfloat scale = 0.5f;

char exePath[MAX_PATH];

DWORD IMD_LoadFile(BINHANDLE *inFile, BYTE search, BYTE exp);

void InitVideo(int w, int h, int bpp, BYTE ortho)
{
	GLdouble xmin, xmax, ymin, ymax;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GEQUAL, 0.2f);

	glViewport(0,0,w,h);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	if(!ortho)
	{
		// setup perspective frustrum
		ymax = 0.1f * tan(45.0f * M_PI / 360.0);
		ymin = -ymax;
		xmin = ymin * (GLfloat)w/(GLfloat)h;
		xmax = ymax * (GLfloat)w/(GLfloat)h;
		glFrustum(xmin, xmax, ymin, ymax, 0.1f, 500.0f);
		glEnable(GL_DEPTH_TEST);							// Enables Depth Testing	
		glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	}
	else
	{
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glOrtho( 0, w, 0, h, -1, 1 );
		glDisable( GL_DEPTH_TEST ); 
	}
	
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 1.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup	
	
	glFrontFace(GL_CW);	
	glMatrixMode(GL_PROJECTION);
	glPointSize(2.0);

	glEnable( GL_TEXTURE_2D );	
}

void IMD_DrawBBox(IMD_SIMPLE_VERT *bbox)
{
	glDisable(GL_TEXTURE_2D);
	glColor3ub(255, 0, 255);
	glBegin(GL_POINTS);
		glVertex3f(bbox[0].x, bbox[0].y, bbox[0].z);
		glVertex3f(bbox[1].x, bbox[1].y, bbox[1].z);		
		glVertex3f(bbox[2].x, bbox[2].y, bbox[2].z);
		glVertex3f(bbox[3].x, bbox[3].y, bbox[3].z);
		glVertex3f(bbox[4].x, bbox[4].y, bbox[4].z);
		glVertex3f(bbox[5].x, bbox[5].y, bbox[5].z);		
		glVertex3f(bbox[6].x, bbox[6].y, bbox[6].z);
		glVertex3f(bbox[7].x, bbox[7].y, bbox[7].z);
	glEnd();	
	glColor3ub(255, 255, 255);
	glEnable(GL_TEXTURE_2D);
}

void IMD_DrawLink(IMD_SIMPLE_VERT *bbox)
{
	glDisable(GL_TEXTURE_2D);
	glColor3ub(0, 0, 255);
	glBegin(GL_POINTS);
		glVertex3f(bbox[0].x, bbox[0].y, bbox[0].z);
		glVertex3f(bbox[1].x, bbox[1].y, bbox[1].z);		
		glVertex3f(bbox[2].x, bbox[2].y, bbox[2].z);
		glVertex3f(bbox[3].x, bbox[3].y, bbox[3].z);
		glVertex3f(bbox[4].x, bbox[4].y, bbox[4].z);
	glEnd();	
	glColor3ub(255, 255, 255);
	glEnable(GL_TEXTURE_2D);
}



GLuint IMD_DrawBlocks(DWORD numSegs, BYTE exp)
{	
	DWORD i = 0;
	DWORD j = 0;

	DWORD flagTest = 0;
	IMD_SEG *curSeg = NULL;
	GLuint displayList;
	char outstring[255];

	FILE *log = NULL;
	BYTE listFlag = 0;


	IMD_VERT_LIST *vList = NULL;
	IMD_COL_VERT_LIST *vColList = NULL;

	if(dump)
		log = fopen("imd_log.txt", "w");
	displayList = glGenLists(1);
	glNewList(displayList,GL_COMPILE);
	if(exp)
		//OBJBegin("test.obj");
	
	resetSegBase();
	resetTexBase();
	resetVListBase();
	resetColVListBase();
	
	curSeg = getSeg();
	while(curSeg != NULL)
	{		
		IMD_DrawBBox(curSeg->hitbox);
		IMD_DrawLink(curSeg->link.verts);
		setVListBase(curSeg->vertLists);
		vList = getVList();
		while(vList != NULL)
		{
			if(exp)
				OBJSetTexture(vList->texID);
			else
				glBindTexture(GL_TEXTURE_2D, vList->texID);		

			if(dump)
			{
				sprintf(outstring, "Seg: %i\n\n", i);
				fputs(outstring, log);
			}

			j = 0;
			if(vList == NULL)
				break;
			if(vList->header != NULL)
			{				
				while( j < vList->realNumVerts)
				{
					if((DWORD)vList->verts[j].flag == 0x00000001 && 
						(DWORD)vList->verts[j-1].flag != 0x00000001)
					{						
						if(listFlag == 1)
						{
							if(dump)
							fputs("\n\nglEnd:\n\n", log);
							if(exp)
								OBJEndStrip();
							else
								glEnd();
						}
						listFlag = 1;
						if(!exp)
							glBegin(GL_TRIANGLE_STRIP);
						if(dump)
						fputs("\n\nglBegin:\n\n", log);
					}
					if(FALSE_COLOR)
					{
						if((DWORD)vList->verts[j].flag == 0x00000001)
							glColor3f(1.0f, 0.0f, 0.0f);
						else if((DWORD)vList->verts[j].flag == 0x3F800000)
							glColor3f(0.0f, 1.0f, 0.0f);
						else if((DWORD)vList->verts[j].flag == 0xBF800000)
							glColor3f(0.0f, 0.0f, 1.0f);
						else
							glColor3f(1.0f, 1.0f, 1.0f);
					}

					if(dump)
					{
						sprintf(outstring, "\nj: %i\nglVertex3f: %f, %f, %f Flag: %x\n glNormal3f: %f, %f, %f Flag: %x\nglTexCoord2 %f, %f Flag: %x\n",  
								j, vList->verts[j].x, vList->verts[j].y, vList->verts[j].z, 
								(DWORD)vList->verts[j].flag, vList->verts[j].normalx, vList->verts[j].normaly, vList->verts[j].normalz,
								(DWORD)vList->verts[j].normalflag,
								vList->verts[j].texcoordx, vList->verts[j].texcoordy, (DWORD)vList->verts[j].texcoordflag);
						fputs(outstring, log);
					}
					
					if(exp)
						OBJUpdate(&vList->verts[j]);
					else
					{
						glTexCoord2f(vList->verts[j].texcoordx, vList->verts[j].texcoordy);
						glNormal3f(vList->verts[j].normalx, vList->verts[j].normaly, vList->verts[j].normalz);
						glVertex3f(vList->verts[j].x, vList->verts[j].y, vList->verts[j].z);
					}
					
					j++;
				}				
			}
			if(dump)
				fputs("\n\nglEnd:\n\n", log);
			listFlag = 0;
			if(exp)
				OBJEndStrip();
			else
				glEnd();
			vList = nextVList();

		}
		
		j = 0;
		listFlag = 0;

		setColVListBase(curSeg->vertColLists);
		vColList = getColVList();
		while(vColList != NULL)
		{
			if(vColList->texID >=1)
			{
				glBindTexture(GL_TEXTURE_2D, vColList->texID);		
			}

			if(dump)
			{
				sprintf(outstring, "\n\nSeg: %i\n", i);
				fputs(outstring, log);
			}

			j = 1;
			if(vColList == NULL)
				break;
			if(vColList->header != NULL)
			{				
				while( j < vColList->realNumVerts)
				{
					if((DWORD)vColList->verts[j].flag == 0x00000001)
					{
						if((DWORD)vColList->verts[j-1].flag != 1)
						{
							if(listFlag == 1)
							{
								if(dump)
									fputs("\n\nglEnd:\n\n", log);
								glEnd();
							}
							listFlag = 1;
							glBegin(GL_TRIANGLE_STRIP);
							if(dump)
								fputs("\n\nglBegin:\n\n", log);
						}
					}

					if(dump)
					{
						sprintf(outstring, "\nj: %i\n(colored)glVertex3f: %f, %f, %f Flag: %x\n glNormal3f: %f, %f, %f Flag: %x\nglTexCoord2 %f, %f Flag: %x\n",  
								j, vColList->verts[j].x, vColList->verts[j].y, vColList->verts[j].z, 
								(DWORD)vColList->verts[j].flag, vColList->verts[j].normalx, vColList->verts[j].normaly, vColList->verts[j].normalz,
								(DWORD)vColList->verts[j].normalflag,
								vColList->verts[j].texcoordx, vColList->verts[j].texcoordy, (DWORD)vColList->verts[j].texcoordflag);
						fputs(outstring, log);
					}
					
					glTexCoord2f(vColList->verts[j].texcoordx, vColList->verts[j].texcoordy);
					glNormal3f(vColList->verts[j].normalx, vColList->verts[j].normaly, vColList->verts[j].normalz);
					glVertex3f(vColList->verts[j].x, vColList->verts[j].y, vColList->verts[j].z);
					glColor3f(vColList->verts[j].R/256, vColList->verts[j].G/256, vColList->verts[j].B/256);
					
					j++;
				}
			}
			if(dump)
				fputs("\n\nglEnd:\n\n", log);
			listFlag = 0;
			glEnd();
			vColList = nextColVList();

		}
		curSeg = nextSeg();
	}	

	if(dump)
		fclose(log);

	if(exp)
		OBJExport();
	else
		glEndList();

	deleteSegList();
	deleteTexList();
	deleteTexEnvList();

	return displayList;
}


DWORD IMD_Load(char *fn)
{
	BINHANDLE *inFile = NULL;
	inFile = binopen(fn);
	return(IMD_LoadFile(inFile, 1, 0));
}
DWORD IMD_Load(BYTE *data, DWORD size, BYTE exp)
{
	BINHANDLE *inFile = NULL;
	inFile = binopen(data, size);
	return(IMD_LoadFile(inFile, 0, exp));
}

DWORD IMD_LoadFile(BINHANDLE *infile, BYTE search, BYTE exp)
{
	IMD_HEAD1 header;
	
	
	IMD_OFS curDataOfs;				// current offset in 0x01 block (offset to 0x40 blocks)

	IMD_SEG *curSeg = NULL;					// current 0x01 block

	IMD_VERTINFO *curVertHeader = NULL;

	IMD_VERT *curVerts = NULL;

	//FILE *infile = NULL;

	WORD numSegs = 0;	
	DWORD curDataTableOfs;			// current data table offset; from post hit box

	DWORD i = 0;
	DWORD test = 0;

	WORD IMDtype = 0;
	WORD curTag = 0;

	WORD numSegEnts = 0;
	WORD numTextures = 0;

	P6T_TEXTURE *curTex = NULL;
	P6T_TEXTURE *workingTex = NULL;
	P6T_EXPORT exportData;

	GLuint * textures = NULL; 

	IMD_VERT_LIST *curVList = NULL;
	IMD_COL_VERT_LIST *curColVList = NULL;
	char fixedPath[MAX_PATH];
	DAT_FILE loadedFile;
	FILE *mtlFile = NULL;

	GLuint texID;

	char texFN[MAX_PATH];

	curVertHeader = (IMD_VERTINFO*)malloc(sizeof(IMD_VERTINFO));

	// get header
	binread(&header, sizeof(IMD_HEAD1), 1, infile);

	

	// check file type
	binseek(infile, header.typeOffset, BIN_SET);
	binread(&IMDtype, sizeof(WORD), 1, infile);
	binseek(infile, 0xE, BIN_CUR);					// skip padding		
	
	//if(exp)
//		mtlFile = fopen("materials.mtl", "w");

	switch(IMDtype)
	{
		case IMD_TYPE_MESH:
			// need a generic IMD container for this, 0x02 blocks 0x10 data and for unassociated data like
			// light types and anim types that dont have obvious containers otherwise
			//fread(&curData->bbox, sizeof(IMD_VERT)*8, 1, infile); 

			binseek(infile, sizeof(DWORD)*8*4, BIN_CUR);		// skip generic hitbox, we don't need it
			binread(&numSegEnts, sizeof(WORD), 1, infile);		// get numSegEnts(or whatever this is)
			binseek(infile, 0x1E, BIN_CUR);						// skip padding
			binread(&curDataOfs, sizeof(IMD_OFS), 1, infile);		// read first data offset
				
			while(curDataOfs.offset!=NULL)						// offset to beginning of current 0x01 or 0x02 block
			{
				curDataTableOfs = bintell(infile);
				binseek(infile, curDataOfs.offset, BIN_SET);		// seek to block

					binread(&curTag, sizeof(WORD), 1, infile);		// read block tag	
					binseek(infile, 0xE, BIN_CUR);					// skip padding		
					switch(curTag)
					{
						case IMD_BLOCK_SEG:
							printf("\nIMD_BLOCK_SEG\n\n");
							curSeg = (IMD_SEG*)malloc(sizeof(IMD_SEG));
							curSeg->vertLists = NULL;
							curSeg->vertColLists = NULL;
							IMD_Load0x01(infile, curSeg);
							pushSeg(curSeg);
							numSegs++;
						break;

						case IMD_BLOCK_LINK_TBL:
							printf("\nIMD_BLOCK_LINK_TBL\n\n");
							IMD_Load0x02(infile);
						break;

						default:
						break;
					}	
				//pushSeg(curSeg);
				binseek(infile, curDataTableOfs, BIN_SET);
				binread(&curDataOfs, sizeof(IMD_OFS), 1, infile);		// read next data offset
			}
	
			break; // end IMD_TYPE_MESH
			
		case IMD_TYPE_LIGHT:
		case IMD_TYPE_ANIM:
			/*switch(curTag)
			{
				case IMD_BLOCK_TAG_B0:
				case IMD_BLOCK_TAG_B1:
				default:
				break;
			}*/
			//break;
		default:
			// unsupported
			printf("Unsupported IMD type %i\n", IMDtype);
			return(0);
			break;
	}

	//binclose(infile);

	printf("Total Segs: %i\n", numSegs+1);

	resetSegBase();
	resetTexBase();
	resetVListBase();
	resetColVListBase();

	// fix textures
	curTex = getTex();
	workingTex = getTex();

	
	while(workingTex != NULL)
	{
		numTextures++;
		printf("Texture: %i\n", numTextures);
		workingTex = nextTex();
	}

	resetTexBase();

	// load textures
	textures = (GLuint*)malloc(numTextures*sizeof(GLuint));

	glGenTextures(numTextures, &textures[0]);
	i = 0;
	GLenum err = glGetError();
	workingTex = getTex();
	// if texture filename is < 3000, load it and the 8 textures that follow it
	// to support skinning
	while(workingTex != NULL)
	{						
		memset(fixedPath, 0, MAX_PATH);
		sprintf(texFN, "%05i.P6T", workingTex->texID);	// loaded ID
		texID = workingTex->texID;
		workingTex->texFN = texID;
		if(err != GL_INVALID_OPERATION)
			workingTex->texID = textures[i];								// generated ID
		else
			workingTex->texID = i;								// generated ID
		texID = textures[i];		
		if(search==1)
			exportData = P6T_LoadTexture(texFN, texID, exePath, 0);
		else
		{
			sprintf(fixedPath, "P6T\\%s", texFN);
			if(exp)
			{
				FS_Export(fixedPath, exp);
				/*
				fprintf(mtlFile, "newmtl texture%i\n", workingTex->texID);
				if(exp == 2)
					fprintf(mtlFile, "map_Ka %05i.tga\nmap_Kd %05i.tga\n\n", workingTex->texFN, workingTex->texFN); 
					*/
				OBJAddMat(texFN, i);
			}
			else
			{
				loadedFile = FS_RequestFile(fixedPath);
				exportData = P6T_LoadTexture(loadedFile.file, texID, 0, true);
			}
			
		}
		workingTex = nextTex();		
		i++;
	}
	//resetSegBase();
	resetTexBase();
	//resetVListBase();
	
	curSeg = getSeg();
	// assign texture ID's
	while(curSeg != NULL)
	{
		setVListBase(curSeg->vertLists);
		curVList = getVList();
			while(curVList != NULL)
			{
				workingTex = getTex();
				while(workingTex != NULL)
				{
					if(workingTex->texFN == curVList->texID)
					{
						curVList->texID = workingTex->texID;
						resetTexBase();
						break;
					}					
					workingTex = nextTex();
				}
				curVList = nextVList();
			}
		resetTexBase();
		setColVListBase(curSeg->vertColLists);
		curColVList = getColVList();
			while(curColVList != NULL)
			{
				workingTex = getTex();
				while(workingTex != NULL)
				{
					if(workingTex->texFN == curColVList->texID)
					{
						curColVList->texID = workingTex->texID;
						resetTexBase();
						break;
					}					
					workingTex = nextTex();
				}
				curColVList = nextColVList();
			}
		curSeg = nextSeg();
	}

	resetSegBase();
	resetTexBase();
	resetVListBase();
	resetColVListBase();
		
	return numSegs;
}


