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
		static void Draw(const std::vector<Vec2>& points, const sf::Color& color, bool screenSpace = false)
		{
			sf::View cur = Game::GetRenderWindow().getView();
			if (screenSpace)
				Game::GetRenderWindow().setView(Game::GetRenderWindow().getDefaultView());
			else
				Game::GetRenderWindow().setView(Game::GetCamera().GetView());
			
			int n = points.size();
			sf::VertexArray vArray(sf::PrimitiveType::Lines, n);
			for (int i = 0; i < n; i++)
			{
				vArray[i].position = sf::Vector2f(points[i].x, points[i].y);
				vArray[i].color = color;
			}
			Game::GetRenderWindow().draw(vArray);
			// restore view
			Game::GetRenderWindow().setView(cur);
		}
	};
}