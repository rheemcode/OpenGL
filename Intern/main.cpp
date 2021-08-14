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
	display->UseVysnc(false);

	Scene::Init();
	Scene* scene = Scene::GetSingleton();
	scene->BeginScene();
	//scene->CreateDefaultActor();

	Time::Create();

	static uint64_t lastTicks = 0;
	static uint32_t frame = 0;
	static uint32_t frames = 0;

	int i = 0;
	while (!Display::isCloseRequest)
	{
		auto ticks = Time::GetSingleton()->GetTicks();
		float delta = float(ticks - lastTicks);
		frame += delta;
		lastTicks = ticks;
		frames++;

		scene->Sync();
		scene->RunLoop(delta / 1000000.f);
		if (i == 0)
		{
			scene->CreateDefaultActor();
			i = 1;
		}
		display->ProcessEvents();
		if (frame > 1000000)
		{
			Console::Log("Frames Drawn:" + std::to_string(frames) + "\n");
			Console::Log("FPS:" + std::to_string(frames) + "\n");
			frame %= 1000000;
			frames = 0;
		}

	}
	scene->Shutdown();
	return 0;
}
