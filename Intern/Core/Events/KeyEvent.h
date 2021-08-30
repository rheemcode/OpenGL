#pragma once

#include "Event.h"
#include "../Input/KeyCode.h"


class GLIB_API KeyEvent : public Event
{
public:
	bool shift;
	bool alt;
	bool control;
	bool pressed;
	int action;
	WPARAM wparam;
	LPARAM lParam;

	void SetKeyCode(KeyCode p_keycode) { m_KeyCode = p_keycode; }
	void SetKeyPressed(bool p_pressed) {p_pressed = pressed; }
	inline KeyCode GetKeyCode() const { return m_KeyCode; }

	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	EVENT_CLASS_TYPE(KeyEvent)

	KeyEvent(KeyCode key = -1)
		: m_KeyCode(key) {}

protected:
	KeyCode m_KeyCode;
};

class GLIB_API  KeyPressedEvent : public KeyEvent
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


class GLIB_API  KeyReleasedEvent : public  KeyEvent
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
