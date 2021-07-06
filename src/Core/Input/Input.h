#pragma once

#include "KeyCode.h"
#include "Mouse.h"
#include "../Events/Event.h"
#include "Math/SimpleVec.h"

class Input
{
	Point2 mousePos;

public:
	
	Point2 GetMousePosition() { return mousePos; }
	void OnEvent(const Event& event);
	void SetMousePos(const Point2& p_pos) { };
	void SetMousePos(const double& p_posX, const double& p_posY) {  }
	void SetMousePos(const int& p_posX, const int& p_posY) { }
	inline static float GetMouseX() {  }
	inline static float GetMouseY() {  }   
	static int GetKeyMods();
	inline static bool IsKeyPressed(KeyCode keyCode) {}
	inline static bool IsMouseButtonPressed(MouseButton button) {  };
	static unsigned int GetKeySym(unsigned int p_code);	
	static unsigned int GetScanSym(unsigned int p_code);
	static Input* GetSingleton();

private:
	static Input* s_Instance;
};

