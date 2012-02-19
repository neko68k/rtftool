// RTFTool.cpp : main project file.

#include "stdafx.h"
#include <string>
#include <vcclr.h>
#include "Form1.h"

#include <Windows.h>
//#include "COpenGL.h"
#include "rtfview/p6t.h"
#include "rtfview/common.h"
#include "rtfview_bridge.h"


#define MAX_PATH 260

using namespace RTFTool;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew Form1());
	return 0;
}

using namespace System;
using namespace System::Globalization;
using namespace System::Runtime::InteropServices;

int openFile(char *inFN)
{
	int dl = 0;
	dl = IMD_DrawBlocks(IMD_Load(inFN), 0);
	return dl;
}

int openFile(BYTE *data, DWORD size)
{
	int dl = 0;
	dl = IMD_DrawBlocks(IMD_Load(data, size, 0), 0);
	return dl;
}

void openFile(BYTE *data, DWORD size, BYTE exp)
{	
	IMD_DrawBlocks(IMD_Load(data, size, exp), exp);	
}


	

