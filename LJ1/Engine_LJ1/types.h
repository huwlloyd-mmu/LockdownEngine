#pragma once

#include "Vec2.h"

#undef MB_RIGHT // defined in windows headers somewhere
namespace LE
{
	// mouse buttons
	enum MouseButtons
	{
		MB_LEFT,
		MB_MIDDLE,
		MB_RIGHT,
		MB_COUNT
	};
}