#pragma once

#include "KeyCode.h"
#include "Mouse.h"
#include "../Events/Event.h"
#include "../Events/KeyEvent.h"
#include "Math/Vector2.h"

class Input
{
	static Point2 mousePos;
	struct KeyPresses
	{
		uint32_t key;
		bool pressed;
	};
	static KeyPresses keys[88];

	struct MouseDown
	{
		MouseButton button;
		bool pressed;
	};
	

	static MouseDown mouseButtons[3];

	static struct MouseAxis
	{
		float x, y;
	} mouseAxis;

public:
	enum Axis
	{
		AXIS_X,
		AXIS_Y
	};

	inline static Point2 GetMousePosition();
	void OnEvent(const Event& event);
	
	static void SetMouseMode(MouseMode p_mouseMode);

	inline static void SetMousePos(const Point2& p_pos);
	inline static void SetMousePos(const int& p_posX, const int& p_posY);
	
	inline static void SetAxis(Axis p_axis, float p_val);
	inline static float GetAxis(Axis);
	inline static float GetAxisRaw(Axis);

	inline static float GetMouseX() {  }
	inline static float GetMouseY() {  }   

	inline static void SetMouseDown(MouseButton p_mouseButton, bool pressed);
	inline static bool GetMouseDown(MouseButton p_mouseButton);
	
	static int GetKeyMods();
	
	inline static void SetKeyDown(KeyCode keyCode, bool pressed);
	inline static bool IsKeyPressed(KeyCode keyCode);
	inline static bool IsMouseButtonPressed(MouseButton button) {  };
	
	static unsigned int GetKeySym(unsigned int p_code);	
	static unsigned int GetScanSym(unsigned int p_code);
	
	static Input* GetSingleton();
	static void Init();
	~Input();
private:
	static Input* s_Instance;
};

inline void Input::SetAxis(Axis p_axis, float p_val)
{
	switch (p_axis)
	{
	case AXIS_X: 
		mouseAxis.x = p_val;
		break;
	case AXIS_Y:
		mouseAxis.y = p_val;
		break;
	default:
		break;
	}
}

inline float Input::GetAxis(Axis p_axis)
{
	switch (p_axis)
	{
	case AXIS_X: 
		return mouseAxis.x;
	case AXIS_Y:
		return mouseAxis.y;
	default:
		return 0;
	}
}
inline float Input::GetAxisRaw(Axis p_axis)
{
	switch (p_axis)
	{
	case AXIS_X: 
	{
		if (mouseAxis.x < 0)
		{
			return -1.f;
		}
		if (mouseAxis.x > 0)
		{
			return 1.f;
		}
		return 0;
	}
	case AXIS_Y:
		if (mouseAxis.y < 0)
		{
			return -1.f;
		}
		if (mouseAxis.y > 0)
		{
			return 1.f;
		}
		return 0;
	default:
		return 0;
	}
}

inline void Input::SetMousePos(const Point2& p_pos)
{
	mousePos = p_pos;
}

inline void Input::SetMousePos(const int& p_posX, const int& p_posY)
{
	mousePos = { static_cast<float>(p_posX), static_cast<float>(p_posY) };
}

inline Point2 Input::GetMousePosition()
{
	return mousePos;
}

inline void Input::SetMouseDown(MouseButton p_mouseButton, bool pressed)
{
	for (int i = 0; i < 3; i++)
	{
		if (p_mouseButton == mouseButtons[i].button)
		{
			mouseButtons[i].pressed = pressed;
			return;
		}
	}
}

inline bool Input::GetMouseDown(MouseButton p_mouseButton)
{
	for (int i = 0; i < 3; i++)
	{
		if (p_mouseButton == mouseButtons[i].button)
		{
			return mouseButtons[i].pressed;
		}
	}

	return false;
}

inline void Input::SetKeyDown(KeyCode keyCode, bool pressed)
{
	for (int i = 0; i < 88; i++)
	{
		if (keyCode == keys[i].key)
		{
			keys[i].pressed = pressed;
			return;
		}
	}
}
inline bool Input::IsKeyPressed(KeyCode keyCode)
{
	for (int i = 0; i < 88; i++)
	{
		if (keyCode == keys[i].key)
		{
			return keys[i].pressed;
		}
	}

	return false;
}