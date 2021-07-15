#pragma once


typedef unsigned char uint8_t;
typedef uint8_t MouseButton;

namespace Mouse
{
	enum : MouseButton
	{
		BUTTON_1 = 1,
		BUTTON_2 = 2,
		BUTTON_3 = 3,
		BUTTON_4 = 4,
		BUTTON_5 = 5,
		BUTTON_6 = 6,
		BUTTON_7 = 7,
		BUTTON_8 = 8,
		LEFT = BUTTON_1,
		MIDDLE = BUTTON_3,
		RIGHT = BUTTON_2,
		LAST = BUTTON_8

	};

}