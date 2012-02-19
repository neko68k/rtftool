#pragma once
#include <Windows.h>
#include <stdio.h>

#define FS_FOLDER	0
#define FS_P6T		1
#define FS_IMD		2
#define FS_DAT		3
#define FS_PDAT		4

namespace RTF_FileSystem
{
    public ref class CFS_Item
	{
	public:
		CFS_Item()
		{
		}
	private:	
		char *fn;
		BYTE type;
	};
};