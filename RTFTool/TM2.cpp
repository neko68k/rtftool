#include <Windows.h>
//#include "tm2.h"


typedef struct
{
	char id[4];	// always TIM2
	DWORD unk[3];
	DWORD lastRow; // points at first byte of last 4 DWORDs
	DWORD unk2;
	DWORD palStart;
	DWORD unk3[2];
	WORD width;
	WORD height;
	DWORD unk4[6];
}TM2HDR;