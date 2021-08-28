#include <glpch.h>
#include "GLApplication.h"

void GLApplication::InitDisplay()
{
	Display* display = Display::GetSingleton();
	display->GetMainWindow()->Init();
	display->GetMainWindow()->BindEventCallback(std::bind(&Scene::OnEvent, this, std::placeholders::_1));
	display->GetMainWindow()->ReleaseCurrent();
	display->GetMainWindow()->MakeCurrent();
}
