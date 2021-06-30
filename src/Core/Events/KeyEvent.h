#pragma once

#include "Event.h"
#include "../Input/KeyCode.h"
#include <Windows.h>


class KeyEvent : public Event
{
public:
	bool shift;
	bool alt;
	bool control;
	int action;
	WPARAM wparam;
	LPARAM lParam;

	inline KeyCode GetKeyCode() const { return m_KeyCode; }

	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
protected:
	KeyEvent(KeyCode key)
		: m_KeyCode(key) {}

	KeyCode m_KeyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent()
	: KeyEvent(-1)
	{}
	KeyPressedEvent(KeyCode keycode, int repeatCount)
		: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

	inline int GetRepeatCount() const { return m_RepeatCount;  }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << m_KeyCode << " {" << m_RepeatCount << " repeats}";
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyPressed)

private:
	int m_RepeatCount;
};


class KeyReleasedEvent : public  KeyEvent
{
public:
	KeyReleasedEvent()
		: KeyEvent(-1) {}
	KeyReleasedEvent(KeyCode keyCode)
		: KeyEvent(keyCode){}


	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << m_KeyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyReleased)

};

class KeyTypedEvent : KeyEvent
{
public:
	KeyTypedEvent(KeyCode keyCode)
		: KeyEvent(keyCode) {}


	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyTypedEvent: " << m_KeyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyTyped)
};
