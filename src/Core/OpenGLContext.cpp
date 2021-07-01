#include <dwmapi.h>
#include "OpenGLContext.h"

#include <GL/wglew.h>

int OpenGLContext::Init(HWND p_Hwnd)
{
	hwnd = p_Hwnd;
	static PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;


	hDC = GetDC(hwnd);

	if (!hDC) {
		return 1; // Return FALSE
	}

	int pixel_format = ChoosePixelFormat(hDC, &pfd);
	if (!pixel_format) // Did Windows Find A Matching Pixel Format?
	{
		return 1; // Return FALSE
	}

	BOOL ret = SetPixelFormat(hDC, pixel_format, &pfd);
	if (!ret) // Are We Able To Set The Pixel Format?
	{
		return 1; // Return FALSE
	}

	hRC = wglCreateContext(hDC);

	if (!hRC) // Are We Able To Get A Rendering Context?
	{
		return 1; // Return FALSE
	}

	wglMakeCurrent(hDC, hRC);
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		return 1;
	}
	
	int attribs[] = {
	WGL_CONTEXT_MAJOR_VERSION_ARB, 4, //we want a 3.3 context
	WGL_CONTEXT_MINOR_VERSION_ARB, 1,
	//and it shall be forward compatible so that we can only use up to date functionality
	WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
	WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB /*| _WGL_CONTEXT_DEBUG_BIT_ARB*/,
	0
	}; //zero indicates the end of the array

	if (wglewIsSupported("WGL_ARB_create_context") == 1)
	{
		HGLRC new_hRC = wglCreateContextAttribsARB(hDC, 0, attribs);
		if (!new_hRC) {
			wglDeleteContext(hRC);
			return 1; // Return false
		}
		wglMakeCurrent(hDC, nullptr);
		wglDeleteContext(hRC);
		hRC = new_hRC;

		if (!wglMakeCurrent(hDC, hRC)) // Try To Activate The Rendering Context
		{
			return 1; // Return FALSE
		}
	}

	return 0;
}

void OpenGLContext::MakeCurrent()
{
	if (!wglMakeCurrent(hDC, hRC))
		MessageBoxA(nullptr, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
}

void OpenGLContext::ReleaseCurrent()
{
	wglMakeCurrent(hDC, nullptr);
}

void OpenGLContext::SwapBuffer()
{
	SwapBuffers(hDC);
}
