#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Vec2.h"
#include "Game.h"

namespace LE
{
	class DebugLines
	{
	public:
		static void Draw(const std::vector<Vec2>& points, const sf::Color& color)
		{
			int n = points.size();
			sf::VertexArray vArray(sf::PrimitiveType::LineStrip, n);
			for (int i = 0; i < n; i++)
			{
				vArray[i].position = sf::Vector2f(points[i].x, points[i].y);
				vArray[i].color = color;
			}
			Game::GetRenderWindow().draw(vArray);
		}
	};
}