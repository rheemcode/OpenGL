#pragma once
#include <Windows.h>
#include <GL/glew.h>

class OpenGLContext
{
	HWND hwnd;
	HDC hDC;
	HGLRC hRC;
	bool isVysnc =false;
	bool isVysncViaCompositor = false;
	int swapInterval = 0;

public:
	int Init(HWND p_Hwnd);
	void MakeCurrent();
	void ReleaseCurrent();
	void SwapBuffer();

	void SetUseVysnc(bool useVysnc);


};