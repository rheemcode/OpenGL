#include "Window/Window.h"
#include "Tests/Torus.h"
#include "Tests/Cube.h"
#include "Tests/Sphere.h"

#include <Windows.h>

#include <Window/Window.h>
#include "Renderer/Debug.h"
#include "Renderer/Renderer.h"
#include <iostream>
#include <fstream>
#include <Tests/Quad.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PWSTR pCmdLine, int nCmdShow)
{
	uint32_t flags = 0;

	Size2 size;
	size.x = 1200;
	size.y = 700;

	Display::Create(hInstance, (WindowFlags)flags, WINDOW_MODE_WINDOWED, size);
	Display* display = Display::GetSingleton();
	display->UseVysnc(true);
	Quad quad;
	quad.AddTextureImage("C:\\Users\\rheen\\source\\repos\\OpenGL\\src\\creepycreep.png");

	Renderer renderer;

	while (!Display::isCloseRequest)
	{
		renderer.BeginScene();
		quad.Draw();
		display->ProcessEvents();
		display->SwapBuffer();
	}
}
