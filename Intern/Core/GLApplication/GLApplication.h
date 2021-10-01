#pragma once

#include "Utils/Debug.h"
#include "GLCore.h"
#include "Scene.h"

#define GL_APPLICATION(X, S)                                       \
		X()														\
		{                                                       \
			applicationName = S;									\
		}                                                       \
		~X()                                                    \
		{                                                       \
			for (auto scene : scenes) {delete scene.second;}   \
			delete s_instance; \
		}

class GLMain;

class GLIB_API GLApplication
{
public:
	enum RenderPipline { FORWARD, DEFFERED };
private:
	RenderPipline renderPipline;

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
	void SetRenderPipline(RenderPipline p_renderPipline) { renderPipline = p_renderPipline; }
	virtual const std::string& GetApplicationName() const { return applicationName; }
	virtual void OnStart() {};
	void AttachScene(Scene* p_scene);
	void SetMainScene(const std::string& sceneName);
	Scene* CreateScene(const std::string& sceneName) const { return new Scene(sceneName); }
	static GLApplication* GetSingleton() { return s_instance; }
};

namespace GLib
{
	GLApplication* CreateApplication();
}