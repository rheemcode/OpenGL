#pragma once
#include "Window/Display.h"
#include "GLApplication.h"
#include "Core/Console.h"
#include "Renderer/Renderer.h"
#include "Timestep.h"


#if defined (ERROR)
	#undef  ERROR
#endif

#define TOWSTR()

extern GLApplication* GLib::CreateApplication();

std::wstring ToWString(const std::string& s)
{
	int slength = (int)s.length() + 1;
	int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

class GLMain
{

public:
	void Init(HINSTANCE hInstance, HINSTANCE prevInstance, PWSTR pCmdLine, int nCmdShow)
	{
		GLApplication* glApplication = GLib::CreateApplication();
		ERR_FAIL_COND_MSG(glApplication == nullptr, "Application not found");
		glApplication->s_instance = glApplication;

		uint32_t flags = 0;
		Size2 size = Size2(1200, 700);

		std::wstring temp = ToWString(glApplication->applicationName);
		LPCWSTR applicationName = temp.c_str();
		Display::Create(hInstance, (WindowFlags)flags, applicationName, WINDOW_MODE_WINDOWED, size);
		Display* display = Display::GetSingleton();
		display->UseVysnc(false, MAIN_WINDOW_ID);
		Time::Create();


		glApplication->Init();
	}

	int Start()
	{
		GLApplication* glApplication = GLApplication::GetSingleton();
		
		ERR_FAIL_COND_V_MSG(glApplication == nullptr, "Application not found", EXIT_FAILURE);

		glApplication->OnStart();
		glApplication->Run();
		return EXIT_SUCCESS;
	}
};