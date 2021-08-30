#pragma once


#include "Actor.h"
#include "Scene.h"
#include "GLApplication.h"
#include "Console.h"
#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Window/Display.h"

#include "GLMain.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PWSTR pCmdLine, int nCmdShow)
{
	GLMain main;
	main.Init(hInstance, prevInstance, pCmdLine, nCmdShow);
	return main.Start();
}
