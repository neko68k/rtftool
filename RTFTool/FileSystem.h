#pragma once
#include <Windows.h>
#include <stdio.h>
#define RTF_DAT 0
#define RTF_FAT 1
#define RTF_FAT_PREPEND 0
#define RTF_FAT_NOPREPEND 1

namespace RTF_FileSystem
{
	typedef struct
	{

	}FS_HEADER;

    public ref class CFileSystem
	{
	private:		
		BYTE prepended;
		DWORD numEntries;

	public:
		CFileSystem(char *inFN, BYTE type)
		{
			if(type == RTF_DAT)
				this->prepended = RTF_FAT_PREPEND;
			else if(type == RTF_FAT)
				this->prepended = RTF_FAT_NOPREPEND;
			
			OpenFAT(inFN);
		}
	private:
		void MergeFAT(FILE *inFile)
		{
		}
		void OpenFAT(char *inFN)
		{
			char magic[4];
			char *check = NULL;
			FILE *inFile = NULL;
			inFile = fopen(inFN, "rb");
			if(inFile == NULL)
			{
				printf("FATAL! File not found: %s\n", inFN);
				return;
			}
			fread(&magic, 4, 1, inFile);
			if(strcmp(magic, "FAT_"))
			{
				fclose(inFile);
				printf("Not a valid FAT: %s\n", inFN);				
				check = strrchr(inFN, '.');
				if(!strcmp(check, ".DAT"))
					strcpy(check, ".FAT");
				else if(!strcmp(check, ".FAT"))
					strcpy(check, ".DAT");
				inFile = fopen(inFN, "rb");
				if(inFile == NULL)
				{
					printf("FATAL: File not found: %s\n", inFN);
					return;
				}
				fread(&magic, 4, 1, inFile);
				if(strcmp(magic, "FAT_"))
				{
					fclose(inFile);
					printf("FATAL: No suitible DAT/FAT file found\n");
				}
				else
					printf("FATAL: No suitible files found\n");
			}

			// before we load the associated DAT
			// check prepend flag
			if(this->prepended == 1)
			{
				return;
			}
			// else do the rest
		}
	};	
};



