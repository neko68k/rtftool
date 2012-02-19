#pragma once
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <direct.h>

#include "RTFTool.h"
#include "About.h"
#include "COpenGL.h"
#include "rtfview\p6t.h"
#include "rtfview\DAT.h"
#include "rtfview\TGA.h"



namespace RTFTool {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace OpenGLForm;


	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			this->timer1->Enabled = false;
			 AllocConsole();
			
			HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
			int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
			FILE* hf_out = _fdopen(hCrt, "w");
			setvbuf(hf_out, NULL, _IONBF, 1);
			*stdout = *hf_out;

			HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
			hCrt = _open_osfhandle((long) handle_in, _O_TEXT);
			FILE* hf_in = _fdopen(hCrt, "r");
			setvbuf(hf_in, NULL, _IONBF, 128);
			*stdin = *hf_in;
			

			//
			//TODO: Add the constructor code here
			//
		}
		
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{			
			if (components)
			{				
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	protected: 
	private: System::Windows::Forms::StatusStrip^  statusStrip1;


	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  exportToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  singleToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  allToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  aboutToolStripMenuItem;
	private: COpenGL^ OpenGL;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::TreeView^  treeView1;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem2;
	private: System::Windows::Forms::ToolStripMenuItem^  childrenToolStripMenuItem;

	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  exportToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  exportIMDForModelingToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  flippedToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  nonflippedTGAToolStripMenuItem;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel1;

	private: System::ComponentModel::IContainer^  components;



	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exportToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->singleToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->allToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->childrenToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->flippedToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->nonflippedTGAToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exportIMDForModelingToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->treeView1 = (gcnew System::Windows::Forms::TreeView());
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->exportToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripStatusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->menuStrip1->SuspendLayout();
			this->statusStrip1->SuspendLayout();
			this->contextMenuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->fileToolStripMenuItem, 
				this->helpToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(854, 24);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->openToolStripMenuItem, 
				this->exportToolStripMenuItem, this->toolStripMenuItem1, this->exitToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(35, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::openToolStripMenuItem_Click);
			// 
			// exportToolStripMenuItem
			// 
			this->exportToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->singleToolStripMenuItem, 
				this->allToolStripMenuItem, this->toolStripMenuItem2, this->childrenToolStripMenuItem, this->exportIMDForModelingToolStripMenuItem});
			this->exportToolStripMenuItem->Name = L"exportToolStripMenuItem";
			this->exportToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->exportToolStripMenuItem->Text = L"Export";
			// 
			// singleToolStripMenuItem
			// 
			this->singleToolStripMenuItem->Name = L"singleToolStripMenuItem";
			this->singleToolStripMenuItem->Size = System::Drawing::Size(188, 22);
			this->singleToolStripMenuItem->Text = L"Selected";
			this->singleToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::singleToolStripMenuItem_Click);
			// 
			// allToolStripMenuItem
			// 
			this->allToolStripMenuItem->Name = L"allToolStripMenuItem";
			this->allToolStripMenuItem->Size = System::Drawing::Size(188, 22);
			this->allToolStripMenuItem->Text = L"All";
			this->allToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::allToolStripMenuItem_Click);
			// 
			// toolStripMenuItem2
			// 
			this->toolStripMenuItem2->Name = L"toolStripMenuItem2";
			this->toolStripMenuItem2->Size = System::Drawing::Size(185, 6);
			// 
			// childrenToolStripMenuItem
			// 
			this->childrenToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->flippedToolStripMenuItem, 
				this->nonflippedTGAToolStripMenuItem});
			this->childrenToolStripMenuItem->Name = L"childrenToolStripMenuItem";
			this->childrenToolStripMenuItem->Size = System::Drawing::Size(188, 22);
			this->childrenToolStripMenuItem->Text = L"Convert P6T";
			// 
			// flippedToolStripMenuItem
			// 
			this->flippedToolStripMenuItem->CheckOnClick = true;
			this->flippedToolStripMenuItem->Name = L"flippedToolStripMenuItem";
			this->flippedToolStripMenuItem->Size = System::Drawing::Size(153, 22);
			this->flippedToolStripMenuItem->Text = L"Flipped TGA";
			this->flippedToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::flippedToolStripMenuItem_Click);
			// 
			// nonflippedTGAToolStripMenuItem
			// 
			this->nonflippedTGAToolStripMenuItem->CheckOnClick = true;
			this->nonflippedTGAToolStripMenuItem->Name = L"nonflippedTGAToolStripMenuItem";
			this->nonflippedTGAToolStripMenuItem->Size = System::Drawing::Size(153, 22);
			this->nonflippedTGAToolStripMenuItem->Text = L"Non-flipped TGA";
			this->nonflippedTGAToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::nonflippedTGAToolStripMenuItem_Click);
			// 
			// exportIMDForModelingToolStripMenuItem
			// 
			this->exportIMDForModelingToolStripMenuItem->CheckOnClick = true;
			this->exportIMDForModelingToolStripMenuItem->Name = L"exportIMDForModelingToolStripMenuItem";
			this->exportIMDForModelingToolStripMenuItem->Size = System::Drawing::Size(188, 22);
			this->exportIMDForModelingToolStripMenuItem->Text = L"Export IMD for Modeling";
			this->exportIMDForModelingToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::exportIMDForModelingToolStripMenuItem_Click);
			// 
			// toolStripMenuItem1
			// 
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			this->toolStripMenuItem1->Size = System::Drawing::Size(149, 6);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->exitToolStripMenuItem->Text = L"Exit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::exitToolStripMenuItem_Click);
			// 
			// helpToolStripMenuItem
			// 
			this->helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->aboutToolStripMenuItem});
			this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
			this->helpToolStripMenuItem->Size = System::Drawing::Size(41, 20);
			this->helpToolStripMenuItem->Text = L"Help";
			// 
			// aboutToolStripMenuItem
			// 
			this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
			this->aboutToolStripMenuItem->Size = System::Drawing::Size(102, 22);
			this->aboutToolStripMenuItem->Text = L"About";
			this->aboutToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::aboutToolStripMenuItem_Click);
			// 
			// statusStrip1
			// 
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->toolStripStatusLabel1});
			this->statusStrip1->Location = System::Drawing::Point(0, 513);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(854, 22);
			this->statusStrip1->TabIndex = 1;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// timer1
			// 
			this->timer1->Interval = 10;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// treeView1
			// 
			this->treeView1->ContextMenuStrip = this->contextMenuStrip1;
			this->treeView1->Location = System::Drawing::Point(12, 27);
			this->treeView1->Name = L"treeView1";
			this->treeView1->Size = System::Drawing::Size(170, 483);
			this->treeView1->TabIndex = 2;
			this->treeView1->NodeMouseDoubleClick += gcnew System::Windows::Forms::TreeNodeMouseClickEventHandler(this, &Form1::treeView1_NodeMouseDoubleClick);
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->exportToolStripMenuItem1});
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(153, 48);
			// 
			// exportToolStripMenuItem1
			// 
			this->exportToolStripMenuItem1->Name = L"exportToolStripMenuItem1";
			this->exportToolStripMenuItem1->Size = System::Drawing::Size(152, 22);
			this->exportToolStripMenuItem1->Text = L"Export";
			this->exportToolStripMenuItem1->Click += gcnew System::EventHandler(this, &Form1::exportToolStripMenuItem1_Click);
			// 
			// toolStripStatusLabel1
			// 
			this->toolStripStatusLabel1->Name = L"toolStripStatusLabel1";
			this->toolStripStatusLabel1->Size = System::Drawing::Size(0, 17);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(854, 535);
			this->Controls->Add(this->treeView1);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->menuStrip1);
			this->DoubleBuffered = true;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;
			this->MainMenuStrip = this->menuStrip1;
			this->MaximizeBox = false;
			this->Name = L"Form1";
			this->SizeGripStyle = System::Windows::Forms::SizeGripStyle::Hide;
			this->Text = L"RTFTool";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->contextMenuStrip1->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 void *test = NULL;
				 OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;
				 int dl = 0;
				 char inFN[MAX_PATH];	
				 char pathName[MAX_PATH];
				 char tok[2] = "\\";
				// int parent = -1;
				char *shortPath = NULL;
				char *FN = NULL;
				DAT_SPLITPATH split;
				System::String^ sResult;
			
				 //memset(pathName, 0, MAX_PATH-1);
				 //memset(inFN, 0, MAX_PATH-1);
				//openFileDialog1->InitialDirectory = "C:\\Users\\neko\\Documents\\Visual Studio 2008\\Projects\\rtyperipper\\Debug";
				openFileDialog1->Filter = "P6T/IMD|*.P6T;*.IMD|FAT/DAT|*.FAT;*.DAT|IMD|*.IMD|P6T|*.P6T|FAT|*FAT|DAT|*.DAT|All Files|*.*";
				openFileDialog1->FilterIndex = 2;
				openFileDialog1->RestoreDirectory = true;
				if ( openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK )
				{
					pin_ptr<const wchar_t> wch = PtrToStringChars(openFileDialog1->FileName);

					// Convert to a char*
					size_t origsize = wcslen(wch) + 1;
					size_t convertedChars = 0;
					wcstombs_s(&convertedChars, inFN, origsize, wch, _TRUNCATE);
					char *extension = NULL;
					GLuint P6Tview = 0;
					P6T_EXPORT exportData;

					extension = strrchr(inFN, '.');
					this->Text = "RTFTool: " + openFileDialog1->FileName;
					FS_Close();
					if(OpenGL)
					{
						delete OpenGL;
					}
					treeView1->Nodes->Clear();

					if(!strcmp(extension, ".IMD"))
					{
						
						OpenGL = gcnew COpenGL(this, 186, 27, 640, 480, 0);		
						dl = openFile(inFN);
						OpenGL->SetDisplayList(dl);
						this->timer1->Enabled = true;
					}
					else if(!strcmp(extension, ".P6T"))
					{
						
						OpenGL = gcnew COpenGL(this, 186, 27, 640, 480, 1);					
						this->timer1->Enabled = false;
						glClearColor(0.0f, 1.0f, 0.0f, 0.0f) ;
						glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
						glGenTextures(1, &P6Tview);		
						exportData = P6T_LoadTexture(inFN, P6Tview, "", 1);
						//glClear(GL_COLOR_BUFFER_BIT);
						glMatrixMode(GL_MODELVIEW);
						glColor3f(1.0f, 1.0f, 1.0f);
						glBegin(GL_QUADS);
							glTexCoord2f(0, 0);	
							glVertex2i(0, exportData.rect->bottom);
			
							glTexCoord2f(1.0f, 0.0f);
							glVertex2i(exportData.rect->right, exportData.rect->bottom);
			
							glTexCoord2f(1.0f, 1.0f);
							glVertex2i(exportData.rect->right, 0);
			
							glTexCoord2f(0.0f, 1.0f);
							glVertex2i(0, 0);
						glEnd();
						OpenGL->SwapOpenGLBuffers();
					}
					
					else if(!strcmp(extension, ".FAT")||!strcmp(extension, ".DAT"))
					{
						// This is a terrible mess and I'm sure theres a better way
						// but it works so whatever...
						
						FS_Load(inFN, extension);
						
						
						//System::String^ passedFN = gcnew String(FN);
						memset(pathName, 0, MAX_PATH);
						treeView1->BeginUpdate();
						while(FS_EnumerateFAT(pathName)!=NULL)
						{							
							split = getPath(pathName);
						
							DWORD sizeOfPath = 0;
							char *removed = NULL;
							char *outPath = NULL;
							array<TreeNode^>^ parentNode;

							if(split.path != NULL)
							{
								System::String^ passed = gcnew String(split.path);
								outPath = (char*)calloc(1, strlen(split.path)+1);
								memcpy(outPath, split.path, strlen(split.path));
								parentNode = treeView1->Nodes->Find( passed , true);
								while(parentNode->Length != 0)
								{								
									removed = strrchr(outPath, '\\');
		
									if(removed != NULL)
									{
										memset(removed, '\0', 1);
										System::String^ passed = gcnew String(outPath);	
									}
									else
									{
										System::String^ sRemoved = gcnew String(split.FN);
										TreeNode^ childNode = gcnew TreeNode( sRemoved );
										System::String^ sOutPath = gcnew String(outPath);
										childNode->Name =sOutPath;
										parentNode[0]->Nodes->Add( childNode );
										split.FN = NULL;
										split.path = NULL;
										break;
									}
									
								}
								sResult = gcnew String(split.path);
							}				
							if(split.path == NULL && split.FN != NULL)
 								{
 								sResult = gcnew String(split.FN);
 								System::String^ sOutPath = gcnew String(sResult);
 								TreeNode^ childNode = gcnew TreeNode( sResult );
 								childNode->Name = sOutPath;
 								treeView1->Nodes->Add( childNode );
 							} 
							if(split.path != NULL && split.FN != NULL)
							{
								TreeNode^ childNode = gcnew TreeNode( sResult );
								System::String^ sOutPath = gcnew String(outPath);
								System::String^ sOutFN = gcnew String(split.FN);
								childNode->Nodes->Add ( sOutFN );
								childNode->Name = sOutPath;
								treeView1->Nodes->Add( childNode );	
							}
						}
						treeView1->EndUpdate();
						return;
					}
				}
				return;
			}

	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
				OpenGL->Render();
				OpenGL->SwapOpenGLBuffers();
			 }
	private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 exit(0);
			 }
	private: System::Void aboutToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				About^ aboutBox = gcnew About;
				aboutBox->ShowDialog();
			 }
private: System::Void treeView1_NodeMouseDoubleClick(System::Object^  sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^  e) {
			 
			 treeView1->PathSeparator = "";
			 String^ fullPath;
			 BYTE *filePointer = NULL;
			 char *extension;
			 P6T_EXPORT exportData;
			 DAT_FILE loadedFile;

			 FS_ClearLoaded();
			 fullPath = treeView1->SelectedNode->FullPath;

			 pin_ptr<const wchar_t> interPath = PtrToStringChars(fullPath);

			 size_t convertedChars = 0;
			size_t  sizeInBytes = ((fullPath->Length + 1));
			errno_t err = 0;
			GLuint P6Tview = 0;
			char    *convPath = (char *)malloc(sizeInBytes);

			err = wcstombs_s(&convertedChars, 
							convPath, sizeInBytes,
							interPath, sizeInBytes);	
			DWORD dl = 0;

			extension = strrchr(convPath, '.');
			 if(extension == NULL)
				 return;
			 loadedFile = FS_RequestFile(convPath);
			 if(!strcmp(extension, ".P6T"))
					{
						if(OpenGL)
						{
							delete OpenGL;
						}
						OpenGL = gcnew COpenGL(this, 186, 27, 640, 480, 1);					
						this->timer1->Enabled = false;
						glClearColor(0.0f, 1.0f, 0.0f, 0.0f) ;
						glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
						glGenTextures(1, &P6Tview);		
						exportData = P6T_LoadTexture(loadedFile.file, P6Tview, 1, true);
						glClear(GL_COLOR_BUFFER_BIT);
						glMatrixMode(GL_MODELVIEW);
						glColor3f(1.0f, 1.0f, 1.0f);
						glBegin(GL_QUADS);
							glTexCoord2f(0, 0);	
							glVertex2i(0, exportData.rect->bottom);
			
							glTexCoord2f(1.0f, 0.0f);
							glVertex2i(exportData.rect->right, exportData.rect->bottom);
			
							glTexCoord2f(1.0f, 1.0f);
							glVertex2i(exportData.rect->right, 0);
			
							glTexCoord2f(0.0f, 1.0f);
							glVertex2i(0, 0);
						glEnd();
						OpenGL->SwapOpenGLBuffers();
					}
			 if(!strcmp(extension, ".IMD"))
					{
						if(OpenGL)
						{
							delete OpenGL;
						}
						OpenGL = gcnew COpenGL(this, 186, 27, 640, 480, 0);		
						dl = openFile((unsigned char *)loadedFile.file, (unsigned long)loadedFile.size);
						OpenGL->SetDisplayList(dl);
						this->timer1->Enabled = true;
					}
		 }
private: System::Void exportToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) {
			 treeView1->PathSeparator = "";
			 String^ fullPath;
			 BYTE *filePointer = NULL;
			 char *exportFN = NULL;
			 FILE *exportFile = NULL;
			 DWORD filesize = 0;
			 BYTE convertP6T = 0;

			 if(treeView1->SelectedNode == nullptr)
				 return;
			 fullPath = treeView1->SelectedNode->FullPath;

			 pin_ptr<const wchar_t> interPath = PtrToStringChars(fullPath);

			 size_t convertedChars = 0;
			size_t  sizeInBytes = ((fullPath->Length + 1));
			errno_t err = 0;
			GLuint P6Tview = 0;
			char    *convPath = (char *)malloc(sizeInBytes);

			err = wcstombs_s(&convertedChars, 
							convPath, sizeInBytes,
							interPath, sizeInBytes);	
			if(flippedToolStripMenuItem->Checked)
				convertP6T = 1;
			else if(nonflippedTGAToolStripMenuItem->Checked)
				convertP6T = 2;
			else if(!flippedToolStripMenuItem->Checked && !nonflippedTGAToolStripMenuItem->Checked)
				convertP6T = 0;

			if(OpenGL)
				delete(OpenGL);

			OpenGL = gcnew COpenGL(this, 0, 0, 0, 0, 0);	
			FS_Export(convPath, convertP6T);
			delete OpenGL;
		 }
private: System::Void singleToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 exportToolStripMenuItem1_Click(sender, e);
		 }
private: System::Void allToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 BYTE convertP6T = 0;
			 char path[MAX_PATH];
			 strcpy(path, "");
			 if(flippedToolStripMenuItem->Checked)
				convertP6T = 1;
			else if(nonflippedTGAToolStripMenuItem->Checked)
				convertP6T = 2;
			else if(!flippedToolStripMenuItem->Checked && !nonflippedTGAToolStripMenuItem->Checked)
				convertP6T = 0;
			 FS_Export(path, convertP6T);
		 }
private: System::Void flippedToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(nonflippedTGAToolStripMenuItem->Checked)
				 nonflippedTGAToolStripMenuItem->Checked = false;
			 if(exportIMDForModelingToolStripMenuItem->Checked)
				 exportIMDForModelingToolStripMenuItem->Checked = false;

			 toolStripStatusLabel1->Text = "Flipped TGA mode not supported in IMD convert mode. IMD convert disabled!";
		 }
private: System::Void nonflippedTGAToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(flippedToolStripMenuItem->Checked)
				 flippedToolStripMenuItem->Checked = false;
			 toolStripStatusLabel1->Text = "Non-flipped TGA mode enabled";
		 }
private: System::Void exportIMDForModelingToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(flippedToolStripMenuItem->Checked)
				 flippedToolStripMenuItem->Checked = false;
			 if(!nonflippedTGAToolStripMenuItem->Checked)
				 nonflippedTGAToolStripMenuItem->Checked = true;
			 toolStripStatusLabel1->Text = "IMD convert and non-flipped TGA modes enabled";
		 }
};
}

