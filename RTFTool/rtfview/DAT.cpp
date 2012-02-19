#include <Windows.h>
#include <stdio.h>
#include <direct.h>
#include <string.h>
#include <map>
#include <algorithm>
#include "../rtftool.h"
#include "p6t.h"
#include "TGA.h"
#include "common.h"
#include "DAT.h"
#include "OBJExport.h"

typedef struct
{
	// fread(this-MAX_PATH)
	unsigned int datoffset;
	unsigned int datsize;
	unsigned int unknown;
	unsigned int tocoffset;

	// read this from 
	//char fqfilename[MAX_PATH];		// must be tokenized to get path and filename	
} FATENT;

typedef struct
{		
	char magic[4];
	DWORD numfiles;
	BYTE pad[0xEC];
	DWORD startoffset;
	DWORD firsttoc;
	DWORD datoffset;
}FATHDR;

#define RTF_FAT 0
#define RTF_DAT 1

struct cmp_str {
bool operator()(char const *a, char const *b) {
return std::strcmp(a, b) < 0;
}
};

#pragma unmanaged
BYTE type = 0;
static BYTE* FAT = NULL;
static FILE *DAT = NULL;
static FATHDR *FATheader = NULL;
static FATENT *FATbase = NULL;
static FATENT *FATcurrent = NULL;
static HANDLE mmDAT;
static DWORD AllocationGranularity = 0;
static std::map<char*, BYTE*, cmp_str> loadedFiles;

#pragma managed

DAT_SPLITPATH getPath(char *string)
{
	DWORD sizeOfPath = 0;
	DAT_SPLITPATH split;
	char *outString;
	if(strrchr(string, '.'))
	{
		outString = strrchr(string, '\\');
		if(outString != NULL)
		{
			outString = outString + 1;
			split.FN = (char*)calloc(1, strlen(outString)-1);
			split.FN = outString;
		}
		else
		{
			split.path = NULL;
			split.FN = string;
			return split;
		}
	}

	sizeOfPath = strlen(string)-strlen(split.FN);
	split.path = (char*)calloc(1, sizeOfPath+1);	
	memcpy(split.path, string, sizeOfPath);
	return split;
}


void SUP_CopyString(BYTE *inPtr, char *outStr)
{
	char temp[1];
	memcpy(temp, inPtr, 1);
	char *cpyStr = outStr;
	while(strcmp(temp, "\0") != 0)
	{
		memcpy(cpyStr, temp, 1);
		cpyStr++;
		inPtr++;
		memcpy(temp, inPtr, 1);
	}
	memcpy(cpyStr, temp, 1);
}
void FS_Load(char *fn, char* ext)
{	
	SYSTEM_INFO sysInfo;
	FILE *inFile = NULL;
	char *DATfile = NULL;
	DWORD FATsize = 0;
	BYTE *test = NULL;
	


	GetSystemInfo(&sysInfo);
	AllocationGranularity = sysInfo.dwAllocationGranularity;
	inFile = fopen(fn, "rb");

	
	if(!strcmp(ext, ".FAT"))
	{
		fseek(inFile, 0, SEEK_END);
		FATsize = ftell(inFile);
		fseek(inFile, 0, SEEK_SET);
		type = RTF_FAT;
	}
	else
	{
		fseek(inFile, 0xFC, SEEK_SET);
		fread(&FATsize, 4, 1, inFile);
		fseek(inFile, 0, SEEK_SET);		
		type = RTF_DAT;
	}
	FAT = (BYTE*)calloc(1, FATsize);
	fread(FAT, FATsize, 1, inFile);
	

	FATheader = (FATHDR*)FAT;
	FATbase = FATcurrent = (FATENT*)(FAT+FATheader->startoffset);
	if(type==RTF_FAT)
	{
		fclose(inFile);
		DATfile = strrchr(fn, '.');
		strncpy(DATfile, ".DAT", 4);
		DAT = fopen(fn, "rb");
	}
	else if(type==RTF_DAT)
	{
		DAT = inFile;
	}
	printf("DAT ready!\n");
}
char *FS_EnumerateFAT(char *FATpath)
{	
	static DWORD enumIndex = 0;
	FATENT *ent = NULL;
	BYTE *TOCoffset = NULL;	
	memset(FATpath, 0, strlen(FATpath));
	if(enumIndex < FATheader->numfiles)
	{
		ent = &FATbase[enumIndex];
		TOCoffset = FAT+ent->tocoffset;
		SUP_CopyString(TOCoffset, FATpath);
		enumIndex++;
		return(FATpath);
	}
	else
	{
		enumIndex = 0;
		return(NULL);
	}
}
FATENT *FS_FindFile(char *inPath)
{	
	FATENT *ent = NULL;
	BYTE *TOCoffset = NULL;	
	DWORD index = 0;
	char FATpath[MAX_PATH];
	for(index = 0; index < FATheader->numfiles; index++)
	{
		memset(FATpath, 0, MAX_PATH);		
		ent = FATbase+index;
		TOCoffset = FAT+ent->tocoffset;
		SUP_CopyString(TOCoffset, FATpath);
		if(!strcmp(FATpath, inPath))
		{
			return ent;
		}		
	}	
	return(NULL);
}

DAT_FILE FS_RequestFile(char *inPath)
{
	DAT_FILE loadedFile;
	char *path = (char*)malloc(MAX_PATH);
	strcpy(path, inPath);
	std::map<char*, BYTE*, cmp_str>::iterator loadedIT;
	void *view = NULL;
	FATENT *ent = NULL;
	DWORD err = 0;
	printf("Searching...\n");
	loadedIT = loadedFiles.find(path);
	ent = FS_FindFile(path);
	if(loadedIT != loadedFiles.end())
	{
		// file already loaded
		// return mapped view pointer
		printf("file already loaded\n");
		loadedFile.file = (*loadedIT).second;
		loadedFile.size = ent->datsize;
		return(loadedFile);
	
	}
	
	if(ent == NULL)
	{
		loadedFile.file = NULL;
		return(loadedFile);
	}
	
	loadedFile.file = (BYTE*)malloc(ent->datsize);
	if(type == RTF_DAT)
		fseek(DAT, ent->datoffset+FATheader->datoffset, SEEK_SET);
	else if(type == RTF_FAT)
		fseek(DAT, ent->datoffset, SEEK_SET);
	fread(loadedFile.file, ent->datsize, 1, DAT);	
	// store new view/path pair
	printf("File loaded!\n");
	loadedFile.size = ent->datsize;
	loadedFiles[path] = loadedFile.file;
	return(loadedFile);
}

void SUP_Delete(std::pair<const char*, BYTE*> passedPair)
{
	free(passedPair.second);
	free((void*)passedPair.first);
}

void FS_ClearLoaded()
{
	DWORD i = 0;
	for_each(loadedFiles.begin(), loadedFiles.end(), SUP_Delete);
	loadedFiles.clear();
}

void FS_Close()
{
	if(DAT)
		fclose(DAT);
	if(FAT)
		free(FAT);
	FS_ClearLoaded();
	printf("FAT closed\n");
	printf("DAT closed\n");
}

void FS_Export(char *path, BYTE convert)
{
	DAT_FILE inFile;
	DWORD depth = 0;
	static BYTE enumerate = 0;
	FILE *outFile = NULL;
	char *result = NULL;
	//char path[MAX_PATH];
	char *extension = NULL;
	P6T_EXPORT exportData;
	char outFN[MAX_PATH];
	memset(outFN, 0, MAX_PATH);
	static BYTE inprogress = 0;
	char *next_token1 = NULL;
	char *next_token2 = NULL;
	char *next_token = NULL;

	

	if(!strrchr(path, '.') && strcmp(path, ""))
		return;
	
	if(!inprogress)
	{
		_mkdir("export");
		_chdir("export");
	}
	if(!strcmp(path, ""))
	{
		//path = (char*)malloc(MAX_PATH);
		FS_EnumerateFAT(path);	
		enumerate = 1;
	}
	while(strcmp(path, ""))
	{	
		if(inprogress)
			next_token = next_token1;
		else
			next_token = next_token2;
		inFile = FS_RequestFile(path);

		result = strtok_s(path, "\\", &next_token);
		if(inprogress)
			next_token1 = next_token;
		else
			next_token2 = next_token;
		if(strrchr(result, '.'))
		{
			outFile = fopen(result, "wb");
			fwrite(inFile.file, inFile.size, 1, outFile);
			fclose(outFile);
			result = strtok_s(NULL, "\\", &next_token);
		}
		else
		{
			if(!inprogress)
			{
				_mkdir(result);
				_chdir(result);
				depth++;
			}
		}


		while(result!=NULL)
		{
			if(inprogress)
				next_token = next_token1;
			else
				next_token = next_token2;
			result = strtok_s(NULL, "\\", &next_token);
			if(inprogress)
				next_token1 = next_token;
			else
				next_token2 = next_token;
			if(result==NULL)
				return;
			if(extension = strrchr(result, '.'))
			{
				if(convert)
				{
					if(!strcmp(extension, ".P6T"))
					{						
						if(convert == 1)
							exportData = P6T_LoadTexture(inFile.file, NULL, 1, true);
						if(convert == 2)
							exportData = P6T_LoadTexture(inFile.file, NULL, 1, false);
						if(convert)
						{
							strncpy(extension, ".TGA", 4);
							TGA_Save((char*)result, exportData.pixels, exportData.rect->right, exportData.rect->bottom);	
						}
					}
					if(!strcmp(extension, ".IMD"))
					{
						memset(outFN, 0, MAX_PATH);
						inprogress = 1;
						next_token2 = next_token;
						strncpy(outFN, result, extension-result);						
						OBJBegin(outFN);
						openFile(inFile.file, inFile.size, convert);
						next_token = next_token2;
						inprogress = 0;
					}
				}
				else
				{
				outFile = fopen(result, "wb");
				fwrite(inFile.file, inFile.size, 1, outFile);
				fclose(outFile);
				}
				result = strtok_s(NULL, "\\", &next_token);
				if(inprogress)
					next_token1 = next_token;
				else
					next_token2 = next_token;
				//if(!strcmp(result, ""))
					//result = strtok_s(NULL, "\\");
			}
			else
			{
				if(!inprogress)
				{
					_mkdir(result);
					_chdir(result);
					depth++;					
				}
			}
		}
		if(!inprogress)
		{
			for(DWORD j = depth; j > 0; j--)
				_chdir("..");
			strcpy(path, "\0");
		}
		if(enumerate && !inprogress)
		{		
			FS_EnumerateFAT(path);
			FS_ClearLoaded();
			//_chdir("..");
		}
		/*else
		{
			_chdir("..");
			//return;
		}*/
		if(!inprogress)
			depth = 0;
	}
	enumerate = 0;
	if(!inprogress)
		_chdir("..");
}