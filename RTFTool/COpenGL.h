#pragma once
#include "stdafx.h"
#include <windows.h>
#include <GL/gl.h>
#include <math.h>
#include "ArcBall\ArcBall.h"

using namespace System::Windows::Forms;
#pragma unmanaged
const float PI2 = 2.0*3.1415926535f;								// PI Squared

Matrix4fT   Transform   = {  1.0f,  0.0f,  0.0f,  0.0f,				// NEW: Final Transform
                             0.0f,  1.0f,  0.0f,  0.0f,
                             0.0f,  0.0f,  1.0f,  0.0f,
                             0.0f,  0.0f,  0.0f,  1.0f };

Matrix3fT   LastRot     = {  1.0f,  0.0f,  0.0f,					// NEW: Last Rotation
                             0.0f,  1.0f,  0.0f,
                             0.0f,  0.0f,  1.0f };

Matrix3fT   ThisRot     = {  1.0f,  0.0f,  0.0f,					// NEW: This Rotation
                             0.0f,  1.0f,  0.0f,
                             0.0f,  0.0f,  1.0f };

ArcBallT    ArcBall(640.0f, 480.0f);				                // NEW: ArcBall Instance
Point2fT    MousePt;												// NEW: Current Mouse Point
bool        isClicked  = false;										// NEW: Clicking The Mouse?
bool        isRClicked = false;										// NEW: Clicking The Right Mouse Button?
bool        isDragging = false;	
#pragma managed
namespace OpenGLForm
{
	

    public ref class COpenGL:public System::Windows::Forms::NativeWindow
	{

	public:
	

		COpenGL(System::Windows::Forms::Form ^ parentForm, 
				int x, int y, GLsizei iWidth, GLsizei iHeight, BYTE ortho)
		{
			CreateParams^ cp = gcnew CreateParams;
			GLdouble xmin, xmax, ymin, ymax;
			DWORD w = iWidth;
			DWORD h = iHeight;			
			// Set the position on the form

			cp->X = x;
			cp->Y = y;
			cp->Height = iHeight;
			cp->Width = iWidth;

			// Specify the form as the parent.

			cp->Parent = parentForm->Handle;
			//parent1 = parentForm;

			// Create as a child of the specified parent

			// and make OpenGL compliant (no clipping)

			cp->Style = WS_CHILD | WS_VISIBLE | 
						WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

			// Create the actual window

			this->CreateHandle(cp);

			m_hDC = GetDC((HWND)this->Handle.ToPointer());

			if(m_hDC)
				MySetPixelFormat(m_hDC);
			

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
			parent1 = parentForm;
		}
	
		virtual System::Void Render(System::Void)
		{
			if (isRClicked)													// If Right Mouse Clicked, Reset All Rotations
			{
				Matrix3fSetIdentity(&LastRot);								// Reset Rotation
				Matrix3fSetIdentity(&ThisRot);								// Reset Rotation
				Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);		// Reset Rotation
			}

			if (!isDragging)												// Not Dragging
			{
				if (isClicked)												// First Click
				{
					isDragging = true;										// Prepare For Dragging
					LastRot = ThisRot;										// Set Last Static Rotation To Last Dynamic One
					ArcBall.click(&MousePt);								// Update Start Vector And Prepare For Dragging
				}
			}
			else
			{
				if (isClicked)												// Still Clicked, So Still Dragging
				{
					Quat4fT     ThisQuat;

					ArcBall.drag(&MousePt, &ThisQuat);						// Update End Vector And Get Rotation As Quaternion
					Matrix3fSetRotationFromQuat4f(&ThisRot, &ThisQuat);		// Convert Quaternion Into Matrix3fT
					Matrix3fMulMatrix3f(&ThisRot, &LastRot);				// Accumulate Last Rotation Into This One
					Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);	// Set Our Final Transform's Rotation From This One
				}
				else														// No Longer Dragging
					isDragging = false;
			}

			glClearColor(0.0f, 1.0f, 0.0f, 0.0f) ;
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();					
			glTranslatef(0.0f,0.0f-0.7f,-4.0f+1.5f);
			glRotatef(90, 0.0f, 1.0f, 0.0f);
			glRotatef(180, 1.0f, 0.0f, 0.0f);		
			glPushMatrix();													// NEW: Prepare Dynamic Transform
			glMultMatrixf(Transform.M);										// NEW: Apply Dynamic Transform
			glCallList(this->list);
			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
			glFlush();
			parent1->Invalidate();
		}

		System::Void SwapOpenGLBuffers(System::Void)
		{
			SwapBuffers(m_hDC) ;
		}

		private:
		HDC m_hDC;
		HGLRC m_hglrc;
		unsigned int list;
		System::Windows::Forms::Form^ parent1;
		

		protected:
	virtual void WndProc(Message% m)override
	{
		GLfloat xPos;
		GLfloat yPos;
		int mousey = 0;
		
		switch (m.Msg) {
			case WM_RBUTTONDOWN:
				isRClicked = true;
				break;
			case WM_RBUTTONUP:
				isRClicked = false;
				break;
			case WM_LBUTTONDOWN:
				isClicked   = true;
				break;				
			case WM_LBUTTONUP:
				 isClicked  = false;
				 break;		
			case WM_MOUSEMOVE:
				System::IntPtr value;
				System::IntPtr buttons;
				value = m.LParam;
				buttons = m.WParam;

				MousePt.s.X = (GLfloat)LOWORD(value.ToInt32());
				MousePt.s.Y = (GLfloat)HIWORD(value.ToInt32());
				//printf("%f\n%f\n\n", MousePt.s.X, MousePt.s.Y);
				isClicked   = (LOWORD(buttons.ToInt32()) & MK_LBUTTON) ? true : false;
				isRClicked  = (LOWORD(buttons.ToInt32()) & MK_RBUTTON) ? true : false;
				break;
		}
		NativeWindow::WndProc(m);
	}
		~COpenGL(System::Void)
		{
			glClearColor(0.0f, 1.0f, 0.0f, 0.0f) ;
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glDeleteLists(this->list, 2);
			this->DestroyHandle();
		}

		public: void SetDisplayList(unsigned int displayList)
		{
			this->list = displayList;
		}

		private: GLint MySetPixelFormat(HDC hdc)
		{
			PIXELFORMATDESCRIPTOR pfd = { 
				sizeof(PIXELFORMATDESCRIPTOR),    // size of this pfd 
				1,                                // version number 
				PFD_DRAW_TO_WINDOW |              // support window 
				PFD_SUPPORT_OPENGL |              // support OpenGL 
				PFD_DOUBLEBUFFER,                 // double buffered 
				PFD_TYPE_RGBA,                    // RGBA type 
				24,                               // 24-bit color depth 
				0, 0, 0, 0, 0, 0,                 // color bits ignored 
				0,                                // no alpha buffer 
				0,                                // shift bit ignored 
				0,                                // no accumulation buffer 
				0, 0, 0, 0,                       // accum bits ignored 
				16,                               // 32-bit z-buffer     
				0,                                // no stencil buffer 
				0,                                // no auxiliary buffer 
				PFD_MAIN_PLANE,                   // main layer 
				0,                                // reserved 
				0, 0, 0                           // layer masks ignored 
			}; 
    
			GLint  iPixelFormat; 
     
			// get the device context's best, available pixel format match 
			if((iPixelFormat = ChoosePixelFormat(hdc, &pfd)) == 0)
			{
				MessageBox::Show("ChoosePixelFormat Failed");
				return 0;
			}
         
			while(true){
				// make that match the device context's current pixel format 
				if(SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE)
				{
					continue;
				}
    
				if((m_hglrc = wglCreateContext(m_hDC)) == NULL)
				{
					continue;
				}
        
				if((wglMakeCurrent(m_hDC, m_hglrc)) != NULL)
				{
					break;
				}
			
			}
				return 1;
		}
	
	};
};





