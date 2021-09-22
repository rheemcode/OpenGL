#include <glpch.h>
#include "GLApplication.h"
#include "Window/Display.h"
#include "Utils/Profiler.h"


GLApplication* GLApplication::s_instance;
Scene* GLApplication::mainScene;

void GLApplication::Init()
{
	Display* display = Display::GetSingleton();
	display->GetMainWindow()->Init();
	display->GetMainWindow()->BindEventCallback(std::bind(&GLApplication::OnEvent, this, std::placeholders::_1));
	//display->UseVysnc(true, 0);

}



void GLApplication::OnEvent(const Event& p_event)
{ 
	ERR_FAIL_COND_MSG(mainScene == nullptr, "Please Attach A Main Scene")
	mainScene->OnEvent(p_event); 
}

void GLApplication::Run()
{
	ERR_FAIL_COND_MSG(mainScene == nullptr, "Please Attach A Main Scene!")

	Display* display = Display::GetSingleton();
	uint64_t lastTicks = 0;
	uint32_t frame = 0;
	uint32_t frames = 0;
	std::vector<float> frameTimesVec;
	Profiler profiler;
	while (!Display::isCloseRequest)
	{
		
		//PROFILE_FUNCTION(
		uint64_t ticks = Time::GetSingleton()->GetTicks();
		
		float delta = float(ticks - lastTicks);
		frame += (uint32_t)delta;
		lastTicks = ticks;
		frames++;
		mainScene->OnUpdate(delta / 1000000.f);
		mainScene->Render();
		profiler.Start();
		display->ProcessEvents();
		frameTimesVec.push_back(profiler.End());
		if (frame > 1000000)
		{
			Console::Log(LogMode::DEBUG, "FPS:" + std::to_string(frames) + "\n");
			//Console::Log(LogMode::DEBUG, "Delta:" + std::to_string(delta / 1000000.f) + "\n");
			frame %= 1000000;
			frames = 0;

			float frameTimes = 0;
			int count = 0;
			for (auto frameTime : frameTimesVec)
			{
				frameTimes += frameTime;
				count++;
			}

			float averageTime = frameTimes / count;
			frameTimesVec.clear();
			Console::Log(LogMode::DEBUG, "Average Function Time in: [Application::Run] is: " + std::to_string(averageTime) + "ms\n");
		}
	}
}

void GLApplication::AttachScene(Scene* scene)
{
	scenes[scene->GetSceneName()] = scene;
	scene->InitScene();
}

void GLApplication::SetMainScene(const std::string& sceneName)
{
	const auto& val = scenes.find(sceneName);
	ERR_FAIL_COND_MSG(val == scenes.end(), "Could'nt Set Main Scene")

	mainScene = val->second;
	mainScene->s_activeScene = mainScene;
}