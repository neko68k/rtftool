#ifndef __DAT_H__
#define __DAT_H__

typedef struct
{
	char *path;
	char *FN;
}DAT_SPLITPATH;

typedef struct
{
	BYTE *file;
	DWORD size;
}DAT_FILE;

void FS_Close();
DAT_FILE FS_RequestFile(char *inPath);
char *FS_EnumerateFAT(char *FATpath);
void FS_Load(char *fn, char *ext);
DAT_SPLITPATH getPath(char *string);
void FS_ClearLoaded();
void FS_Export(char *path, BYTE convert);


#endif //__DAT_H__



