#pragma once

#include "GLCore.h"
#include "Scene.h"

#define GL_APPLICATION(X)                                       \
		X()														\
		{                                                       \
		    auto* scene = CreateScene(#X);                      \
			applicationName = std::string(#X);					\
			AttachScene(scene);									\
			SetMainScene(#X);									\
		}                                                       \
		~X()                                                    \
		{                                                       \
			for (auto scene : scenes) {delete scene.second;}   \
			delete s_instance; \
		}

class GLMain;

class GLIB_API GLApplication
{
protected:
	friend GLMain;

	static GLApplication* s_instance;
	std::unordered_map<std::string, Scene*> scenes;
	static Scene* mainScene;

	void Init();
	void OnEvent(const Event& p_event);
	void Run();

	std::string applicationName;

public:
	virtual const std::string& GetApplicationName() { return applicationName; }

	void AttachScene(Scene* p_scene);
	void SetMainScene(const std::string& sceneName);
	Scene* CreateScene(const std::string& sceneName) { return new Scene(sceneName); }
	static GLApplication* GetSingleton() { return s_instance; }
};

namespace GLib
{
	GLApplication* CreateApplication();
}