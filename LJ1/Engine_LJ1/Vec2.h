#pragma once

namespace LE
{
	// a 2d vector class
	class Vec2
	{
	public:
		float x;
		float y;

		Vec2() : x(0.0f), y(0.0f) {}
		Vec2(float x, float y) : x(x), y(y) {}
		Vec2(const Vec2& other) : x(other.x), y(other.y) {}
		// that'll do for now.
	};
}