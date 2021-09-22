#include <glpch.h>
#include "OpenGLContext.h"

#include <GL/wglew.h>




int OpenGLContext::Init(HWND p_Hwnd)
{
	hwnd = p_Hwnd;
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
		PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
		32,                   // Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                   // Number of bits for the depthbuffer
		8,                    // Number of bits for the stencilbuffer
		0,                    // Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};


	hDC = GetDC(hwnd);

	if (!hDC) {
		return 1; // Return FALSE
	}
	
	int pixelFormat = 0;
	UINT numFormat = 0;

	const int iPixelFormatAttribList[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB, 32,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
			WGL_SAMPLES_ARB, 1,
			0 // End of attributes list
	};
	

	
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
	//wglChoosePixelFormatARB(hDC, iPixelFormatAttribList, NULL, 1, &pixelFormat, &numFormat);
//	BOOL ret = SetPixelFormat(hDC, pixelFormat, &pfd);
	if (!ret) // Are We Able To Set The Pixel Format?
	{
		return 1; // Return FALSE
	}
	int attribs[] = {
	WGL_CONTEXT_MAJOR_VERSION_ARB, 4, //we want a 4.1 context
	WGL_CONTEXT_MINOR_VERSION_ARB, 1,

	//and it shall be forward compatible so that we can only use up to date functionality
	WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
	WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB /*| _WGL_CONTEXT_DEBUG_BIT_ARB*/,
	0, 0
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

bool vsyncViaCompositor() {

	BOOL dwm_enabled;

	if (SUCCEEDED(DwmIsCompositionEnabled(&dwm_enabled))) {
		return dwm_enabled;
	}

	return false;
}

void OpenGLContext::SwapBuffer()
{

	if (isVysnc)
	{
		bool vcmp = vsyncViaCompositor();
		if (vcmp && wglGetSwapIntervalEXT() == 0)
			DwmFlush();

		if (vcmp != isVysncViaCompositor)
			SetUseVysnc(true);
	}

	if (!SwapBuffers(hDC))
	{
		DWORD error = GetLastError();
		std::ofstream fs("log_output", std::ios_base::app);
		fs << error << std::endl;
		fs.flush();
		fs.close();

	}

}

void OpenGLContext::SetUseVysnc(bool useVysnc)
{
	isVysncViaCompositor = useVysnc && vsyncViaCompositor();

	if (wglSwapIntervalEXT) {
		int swap_interval = (useVysnc && !isVysncViaCompositor) ? 1 : 0;
		wglSwapIntervalEXT(swap_interval);
	}

	isVysnc = useVysnc;
}
