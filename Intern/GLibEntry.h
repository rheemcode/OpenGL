#pragma once

#include "GLMain.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PWSTR pCmdLine, int nCmdShow)
{
	GLMain main;
	main.Init(hInstance, prevInstance, pCmdLine, nCmdShow);
	return main.Start();
}
