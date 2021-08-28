#pragma once
#include "GLib.h"
#include "Scene.h"

class GLApplication
{
	static GLApplication* application;
	static Scene* mainScene;
	void InitDisplay();

public:
	void AttachScene();
	void OnUpdate(float p_deltaTime);
	void OnEvent();
};

static GLApplication* CreateApplication()
{
	return nullptr;
}