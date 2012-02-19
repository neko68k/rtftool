#include <windows.h>
#include <stdio.h>
#include <direct.h>
#include <set>
#include <vector>
#include <algorithm>
#include "common.h"
#include "OBJExport.h"
#include "linked_lists.h"

	typedef struct{
		GLfloat x;
		GLfloat y;
		GLfloat z;
	}vertex;
	
	typedef struct{
		GLfloat u;
		GLfloat v;
		GLfloat padding;
	}texcoord;

	typedef struct{
		GLfloat x;
		GLfloat y;
		GLfloat z;
	}normal;

	typedef struct{
		vertex vert[3];
		texcoord tex[3];
		normal norm[3];

		DWORD vertInd[3];
		DWORD texInd[3];
		DWORD normalInd[3];
		GLuint texID;
	}triangle;



#pragma unmanaged

std::vector<vertex*> allVertices;
std::vector<texcoord*> allTexCoord;
std::vector<normal*> allNormals;

std::vector<triangle*> allTriangles;
std::vector<IMD_VERT*> strip;
FILE *outFile = NULL;
FILE *outMat = NULL;
GLuint currentTex = 0;
#pragma managed

// open the file for export and reset memory structures
void OBJBegin(char *outfn)
{
	char *extension = NULL;
	strcat(outfn, ".obj\0");
	outFile = fopen(outfn, "w");
	extension = strrchr(outfn, '.');
	strcpy(extension, ".mtl\0");
	outMat = fopen(outfn, "w");
	fprintf(outFile, "mtllib %s\n", outfn);
	allVertices.clear();
	allTexCoord.clear();
	allNormals.clear();
	allTriangles.clear();
	strip.clear();
}

void OBJAddMat(char *fn, GLuint id)
{
	char *extension = strrchr(fn, '.');
	strcpy(extension, ".tga\0");
	fprintf(outMat, "newmtl texture%i\n", id);		
	fprintf(outMat, "map_Ka %s\nmap_Kd %s\n\n", fn, fn); 

}

// end strip and process verts into triangles
void OBJEndStrip()
{
	triangle *workingTri = (triangle*)malloc(sizeof(triangle));
	bool odd = true;
	for(int i = 0; i<strip.size()-2; i++)
	{
		//fprintf(outFile, "Face: %i\n", i);
		workingTri = (triangle*)malloc(sizeof(triangle));
		if(!odd)
		{
			for(int j = 0; j < 3; j++)
			{
				workingTri->vert[j].x = strip[i+j]->x;
				workingTri->vert[j].y = strip[i+j]->y;
				workingTri->vert[j].z = strip[i+j]->z;

				workingTri->tex[j].u = strip[i+j]->texcoordx;
				workingTri->tex[j].v = strip[i+j]->texcoordy;

				workingTri->norm[j].x = strip[i+j]->normalx;
				workingTri->norm[j].y = strip[i+j]->normaly;
				workingTri->norm[j].z = strip[i+j]->normalz;
				//fprintf(outFile, "%i: %f, %f, %f\n", j, workingTri->vert[j].x, workingTri->vert[j].y, workingTri->vert[j].z);
			}
			odd = true;
		}		
		else
		{
			for(int j = 2; j > -1; j--)
			{
				workingTri->vert[j].x = strip[(i+2)-j]->x;
				workingTri->vert[j].y = strip[(i+2)-j]->y;
				workingTri->vert[j].z = strip[(i+2)-j]->z;

				workingTri->tex[j].u = strip[(i+2)-j]->texcoordx;
				workingTri->tex[j].v = strip[(i+2)-j]->texcoordy;

				workingTri->norm[j].x = strip[(i+2)-j]->normalx;
				workingTri->norm[j].y = strip[(i+2)-j]->normaly;
				workingTri->norm[j].z = strip[(i+2)-j]->normalz;
			}
			odd = false;
		}
		workingTri->texID = currentTex;
		allTriangles.push_back(workingTri);
		//fprintf(outFile, "\n\n");
	} 
	strip.clear();
}

// update structures with new data
void OBJUpdate(IMD_VERT *inVert)
{	
		vertex *vert = (vertex*)malloc(sizeof(vertex));
		vert->x = inVert->x;
		vert->y = inVert->y;
		vert->z = inVert->z;

		texcoord *tex = (texcoord*)malloc(sizeof(texcoord));
		tex->u = inVert->texcoordx;
		tex->v = inVert->texcoordy;

		normal *norm = (normal*)malloc(sizeof(normal));
		norm->x = inVert->normalx;
		norm->y = inVert->normaly;
		norm->z = inVert->normalz;
	
		allNormals.push_back(norm);
		allTexCoord.push_back(tex);
		allVertices.push_back(vert);
		IMD_VERT *saveVert = (IMD_VERT*)malloc(sizeof(IMD_VERT));
		memcpy(saveVert, inVert, sizeof(IMD_VERT));
		strip.push_back(saveVert);
}

void findIndex()
{
	for(int k = 0; k < allTriangles.size(); k++)
	{
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < allVertices.size(); j++)
			{
				if( allVertices[j]->x == allTriangles[k]->vert[i].x && 
					allVertices[j]->y == allTriangles[k]->vert[i].y &&
					allVertices[j]->z == allTriangles[k]->vert[i].z)
				{
					allTriangles[k]->vertInd[i] = j+1;
					j=0;
					break;
				}
			}
			for(int j = 0; j < allVertices.size(); j++)
			{
				if( allNormals[j]->x == allTriangles[k]->norm[i].x && 
					allNormals[j]->y == allTriangles[k]->norm[i].y &&
					allNormals[j]->z == allTriangles[k]->norm[i].z)
				{
					allTriangles[k]->normalInd[i] = j+1;
					j=0;
					break;
				}
			}
			for(int j = 0; j < allVertices.size(); j++)
			{
				if( allTexCoord[j]->u == allTriangles[k]->tex[i].u && 
					allTexCoord[j]->v == allTriangles[k]->tex[i].v)
				{
					allTriangles[k]->texInd[i] = j+1;
					j=0;
					break;
				}
			}			
		}
	}
}

void writeVerts(vertex *vert)
{
	fprintf(outFile, "v %f %f %f\n", vert->x, vert->y, vert->z);
}

void writeNormal(normal *norm)
{
	fprintf(outFile, "vn %f %f %f\n", norm->x, norm->y, norm->z);
}

void writeTex(texcoord*tex)
{
	fprintf(outFile, "vt %f %f \n", tex->u, tex->v);
}

void writeFaces(triangle *tri)
{
	static GLint currentTex = -1;
	char outString[65535];
	memset(outString, 0, 65535);	
	char finString[65535];
	memset(finString, 0, 65535);
	sprintf(finString, "f ");
	if(tri->texID != currentTex)
	{
		currentTex = tri->texID;
		fprintf(outFile, "\nusemtl texture%i\n", currentTex);
	}
	for(int i = 0; i < 3; i++)
	{
		sprintf(outString, "%i/%i/%i ", tri->vertInd[i], tri->texInd[i], tri->normalInd[i]);
		strcat(finString, outString);
		memset(outString, 0, 65535);	
	}
	strcat(finString, "\n");
	fprintf(outFile, finString);
}

// process triangles into indexed lists
// perform export file write and reset all structures
void OBJExport()
{
	//printf("%i\n", allVertices.size());
	
	findIndex();
	for_each(allVertices.begin(), allVertices.end(), writeVerts);
	for_each(allNormals.begin(), allNormals.end(), writeNormal);
	for_each(allTexCoord.begin(), allTexCoord.end(), writeTex);
	for_each(allTriangles.begin(), allTriangles.end(), writeFaces);
	allVertices.clear();
	allTexCoord.clear();
	allNormals.clear();
	allTriangles.clear();
	strip.clear();
	fclose(outFile);
	fclose(outMat);
}

void OBJSetTexture(GLuint texid)
{

	currentTex = texid;

}