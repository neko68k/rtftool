/*  imd.cpp

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

#ifdef WIN32
	#include <windows.h>
#endif

#include <stdio.h>
#include "../binfile.h"
#include "common.h"
#include "linked_lists.h"


// split initial table seg loader to handle only other table types(0x01, 0x02, etc)
// let specialized loader below handle all tables referenced
// this should simplify the logic requiered in the main loader and keep code
// readable and maintainable
void IMD_Load0x01(BINHANDLE *infile, IMD_SEG *seg)
{
	WORD curTag = 0;
	IMD_OFS curOfs;
	IMD_OFS oldOfs;

	IMD_VERT_LIST *curVertList = NULL;
	IMD_COL_VERT_LIST *curColVertList = NULL;
	IMD_VERT *workingVert = NULL;

	P6T_TEXTURE *curTexInfo = NULL;
	P6T_TEXTURE_ENV *curTexEnvInfo = NULL;

	FILE *bindump = NULL;

	DWORD i = 0;
	DWORD numSubBlocks = 0;
	DWORD realBlockSize = 0;
	DWORD lastBlockSize = 0;
	DWORD bytesToRead = 0;
	DWORD vertBytes = 0;

	IMD_VERTSUBINFO tempSubInfo;

	// read first offset and tag
	binread(&curOfs, sizeof(IMD_OFS), 1, infile);
	oldOfs.offset = bintell(infile);

	//setVListBase(NULL);
	while(curOfs.offset != NULL)
	{
		binseek(infile, curOfs.offset, BIN_SET);
		binread(&curTag, sizeof(WORD), 1, infile);
		
		switch(curTag)
		{
			case IMD_BLOCK_LINKINFO:
				printf("LINKINFO @ %x from %x\n", curOfs.offset, oldOfs.offset);
				binseek(infile, 0xE, BIN_CUR);						// skip padding
				binread(&seg->link, sizeof(IMD_LINK), 1, infile);
				// if 0x02 block exists, xref start offset, if not loaded, then load and push, set pointer to data in seg struct
			break;

			case IMD_BLOCK_HITBOX:
				printf("HITBOX @ %x from %x\n", curOfs.offset, oldOfs.offset);
				binseek(infile, 0xE, BIN_CUR);						// skip padding
				binread(seg->hitbox, sizeof(IMD_SIMPLE_VERT)*8, 1, infile);
			break;

			case IMD_BLOCK_TEXINFO_MAT:
				printf("Unsupported block type IMD_BLOCK_TEXINFO_MAT @ %x from %x\n", curOfs.offset, oldOfs.offset);
				binseek(infile, 0xE, BIN_CUR);						// skip padding
				// unsupported
			break;

			case IMD_BLOCK_TEXINFO_ENV:
				printf("IMD_BLOCK_TEXINFO_ENV @ %x from %x\n", curOfs.offset, oldOfs.offset);
				binseek(infile, 0xE, BIN_CUR);						// skip padding
				
				curTexInfo = (P6T_TEXTURE*)malloc((sizeof(P6T_TEXTURE)));
				memset(curTexInfo, 0, sizeof(P6T_TEXTURE));
				binread(curTexInfo, (0x10), 1, infile);
				pushTex(curTexInfo);

			break;

			case IMD_BLOCK_TEXINFO23:
			case IMD_BLOCK_TEXINFO:
				printf("TEXINFO @ %x from %x\n", curOfs.offset,oldOfs.offset);
				binseek(infile, 0x0E, BIN_CUR);						// skip padding
				
				
				curTexInfo = (P6T_TEXTURE*)malloc((sizeof(P6T_TEXTURE)));
				memset(curTexInfo, 0, sizeof(P6T_TEXTURE));
				binread(curTexInfo, (sizeof(P6T_TEXTURE)-12), 1, infile);
				pushTex(curTexInfo);
			break;

			case IMD_BLOCK_VERTINFO2:
				curColVertList = (IMD_COL_VERT_LIST*)malloc(sizeof(IMD_COL_VERT_LIST));
				if(getColVList() == NULL)
				{
					setColVListBase(curColVertList);
					seg->vertColLists = curColVertList;
				}
				if(curTexInfo != NULL)		
					curColVertList->texID = curTexInfo->texID;
				else
					curColVertList->texID = 0;

				printf("IMD_VERTINFO2 @ %x from %x\n", curOfs.offset,oldOfs.offset);
				curColVertList->header = (IMD_VERTINFO*)malloc(sizeof(IMD_VERTINFO));				
				binseek(infile, 0x6, BIN_CUR);
				binread(curColVertList->header, sizeof(IMD_VERTINFO), 1, infile);

				binread(&tempSubInfo, sizeof(IMD_VERTSUBINFO), 1, infile); // read first sub info
				numSubBlocks = curColVertList->header->totalVerts/((tempSubInfo.numVerts*4)+4);
				if(curColVertList->header->totalVerts % ((tempSubInfo.numVerts*4)+4) != 0)
					numSubBlocks++;

				bytesToRead = (tempSubInfo.numVerts*4*4*4);

				curColVertList->verts = (IMD_COL_VERT*)malloc(bytesToRead*numSubBlocks);
				curColVertList->realNumVerts = tempSubInfo.numVerts;
				binread(curColVertList->verts, bytesToRead, 1, infile);

				for(i = 1; i<=numSubBlocks-1; i++)
				{			
					
					binseek(infile, 0x10, BIN_CUR); // skip actual 0x17 tag
					binread(&tempSubInfo, sizeof(IMD_VERTSUBINFO), 1, infile); // read next sub info
					bytesToRead = (tempSubInfo.numVerts*4*4*4);					
					binread(&curColVertList->verts[(curColVertList->realNumVerts)], bytesToRead, 1, infile);
					curColVertList->realNumVerts += tempSubInfo.numVerts;
					
				}

				pushColVList(curColVertList);	

				break;
			case IMD_BLOCK_VERTINFO3:
				printf("IMD_VERTINFO3 @ %x from %x\n", curOfs.offset,oldOfs.offset);
				curVertList = (IMD_VERT_LIST*)malloc(sizeof(IMD_VERT_LIST));
				if(getVList() == NULL)
				{
					setVListBase(curVertList);
					seg->vertLists = curVertList;
				}
				if(curTexInfo != NULL)		
					curVertList->texID = curTexInfo->texID;
				else
					curVertList->texID = 0;

				curVertList->header = (IMD_VERTINFO*)malloc(sizeof(IMD_VERTINFO));				
				binseek(infile, 0x6, BIN_CUR);
				binread(curVertList->header, sizeof(IMD_VERTINFO), 1, infile);

				binread(&tempSubInfo, sizeof(IMD_VERTSUBINFO), 1, infile); // read first sub info				
				numSubBlocks = curVertList->header->totalVerts/((tempSubInfo.numVerts*3)+4);
				if(curVertList->header->totalVerts % ((tempSubInfo.numVerts*3)+4) != 0)
					numSubBlocks++;

				bytesToRead = (tempSubInfo.numVerts*3*4*4);

				curVertList->verts = (IMD_VERT*)malloc(bytesToRead*numSubBlocks);
				curVertList->realNumVerts = tempSubInfo.numVerts;
				binread(curVertList->verts, bytesToRead, 1, infile);

				for(i = 1; i<=numSubBlocks-1; i++)
				{			
					binseek(infile, 0x10, BIN_CUR); // skip actual 0x17 tag
					binread(&tempSubInfo, sizeof(IMD_VERTSUBINFO), 1, infile); // read next sub info
					bytesToRead = (tempSubInfo.numVerts*3*4*4);					
					binread(&curVertList->verts[(curVertList->realNumVerts)], bytesToRead, 1, infile);
					curVertList->realNumVerts += tempSubInfo.numVerts;
				}
					
				pushVList(curVertList);	
			break;
			case IMD_BLOCK_VERTINFO:
				printf("IMD_VERTINFO1 @ %x from %x\n", curOfs.offset,oldOfs.offset);
				curVertList = (IMD_VERT_LIST*)malloc(sizeof(IMD_VERT_LIST));
				if(getVList() == NULL)
				{
					setVListBase(curVertList);
					seg->vertLists = curVertList;
				}
				if(curTexInfo != NULL)		
					curVertList->texID = curTexInfo->texID;
				else
					curVertList->texID = 0;

				curVertList->header = (IMD_VERTINFO*)malloc(sizeof(IMD_VERTINFO));				
				binseek(infile, 0x6, BIN_CUR);
				binread(curVertList->header, sizeof(IMD_VERTINFO), 1, infile);

				binread(&tempSubInfo, sizeof(IMD_VERTSUBINFO), 1, infile); // read first sub info				
				numSubBlocks = curVertList->header->totalVerts/((tempSubInfo.numVerts*3)+4);
				if(curVertList->header->totalVerts % ((tempSubInfo.numVerts*3)+4) != 0)
					numSubBlocks++;

				bytesToRead = (tempSubInfo.numVerts*3*4*4);

				curVertList->verts = (IMD_VERT*)malloc(bytesToRead*numSubBlocks);
				curVertList->realNumVerts = tempSubInfo.numVerts;
				binread(curVertList->verts, bytesToRead, 1, infile);

				for(i = 1; i<=numSubBlocks-1; i++)
				{			
					binseek(infile, 0x10, BIN_CUR); // skip actual 0x17 tag
					binread(&tempSubInfo, sizeof(IMD_VERTSUBINFO), 1, infile); // read next sub info
					bytesToRead = (tempSubInfo.numVerts*3*4*4);					
					binread(&curVertList->verts[(curVertList->realNumVerts)], bytesToRead, 1, infile);
					curVertList->realNumVerts += tempSubInfo.numVerts;
				}
					
				pushVList(curVertList);	
			break;	

			
			
			case IMD_BLOCK_TAG_22:
			//case IMD_BLOCK_TAG_23:
			case IMD_BLOCK_TAG_24:
			case IMD_BLOCK_TAG_25:
			case IMD_BLOCK_TAG_26:
			//case IMD_BLOCK_TEXINFO_ENV_TEX:
			
			case IMD_BLOCK_TAG_50:
			case IMD_BLOCK_TAG_52:
			
			case IMD_BLOCK_TAG_60:
			case IMD_BLOCK_TAG_68:
			
				printf("Unsupported block type %x @ %x from %x\n", curTag, curOfs.offset, oldOfs.offset);
			break;

			default:
				printf("Unknown TAG %x @ %x from %x\n", curTag, curOfs.offset, oldOfs.offset-8);
			break;
		}
		binseek(infile, oldOfs.offset, BIN_SET);
		binread(&curOfs, sizeof(IMD_OFS), 1, infile);
		oldOfs.offset = bintell(infile);
	}
}

void IMD_Load0x02(BINHANDLE *infile)
{
	// load while current offset != NULL
	// this is going to require a generic container


	// load table of 0x10 block offsets
	// loop through offsets and load seg data on-the-fly

	return;
}
