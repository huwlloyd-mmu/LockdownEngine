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
		// some arithmetic operators
		Vec2 operator*(const Vec2& other) const { return Vec2(x * other.x, y * other.y); }
		Vec2 operator*(float fac) const { return Vec2(x * fac, y * fac); }
		Vec2 operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
		Vec2 operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
		float magnsqrd() { return x * x + y * y;  }
		float magnitude() { return sqrt(magnsqrd()); }
		Vec2 normalized() { float m = magnitude();  return Vec2(x / m, y / m); }
		void normalize() { float m = magnitude(); x /= m; y /= m; }
		// that'll do for now.
	};
}