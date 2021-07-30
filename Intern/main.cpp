#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <thread>

#include "Window/Window.h"
#include "Tests/Torus.h"
#include "Tests/Cube.h"
#include "Tests/Sphere.h"
#include "Renderer/Scene.h"
#include "Core/Console.h"
#include "Renderer/Debug.h"
#include "Renderer/Renderer.h"
#include "Timestep.h"

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

	Console::Log("HELLO WORLD");

	std::cout << "Hello World";

	Renderer::Init();

	renderer2D.Init();

	std::thread t(&Scene::Process, scene);

	Time::Create();

	static uint64_t lastTicks = 0;
	static uint32_t frame = 0;
	static uint32_t frames = 0;

	while (!Display::isCloseRequest)
	{
		auto ticks = Time::GetSingleton()->GetTicks();
		float delta = ticks - lastTicks;
		frame += delta;
		lastTicks = ticks;
		frames++;

		Renderer::SetClearColor(.4f, .4f, .4f, 1);
		Renderer::Clear();
		scene->OnUpdate();
		display->SwapBuffer();
		display->ProcessEvents();
		
		if (frame > 1000000)
		{
			Console::Log("Delta: " + std::to_string(delta / 1000000) + "\n");
			Console::Log("Frames:" + std::to_string(frames) + "\n");
			Console::Log("FPS:" + std::to_string(frames) + "\n");
			frame %= 1000000;
			frames = 0;
		}
	}
	scene->Shutdown();
	t.join();
}
