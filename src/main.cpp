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
#include <Tests/Sprite.h>
#include <Tests/Quad.h>
#include <Renderer/Scene.h>



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PWSTR pCmdLine, int nCmdShow)
{
	uint32_t flags = 0;

	Size2 size;
	size.x = 1200;
	size.y = 700;

	Display::Create(hInstance, (WindowFlags)flags, WINDOW_MODE_WINDOWED, size);
	Display* display = Display::GetSingleton();
	display->m_Windows[0]->MakeCurrent();
	display->UseVysnc(true);

	Scene* scene = new Scene();


	Renderer2D renderer2D;


	Renderer::Init();

	renderer2D.Init();



	while (!Display::isCloseRequest)
	{
		Renderer::SetClearColor(.4f, .4f, .4f, 1);
		Renderer::Clear();
		scene->OnUpdate();
		display->SwapBuffer();
		display->ProcessEvents();
	}
}
