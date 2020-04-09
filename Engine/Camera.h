#pragma once
#include <SFML/Graphics.hpp>
#include "Vec2.h"
#include "Game.h"

namespace LE
{
	class Camera
	{
		Vec2 pos = Vec2(10.0f,10.0f);
		float width = 20.0f;
		float height = 15.0f;
		sf::View view;
	public:
		Camera() {};
		void SetWidth( float nwidth ) 
		{
			// sets width, calculates height from aspect ratio of screen
			width = nwidth;
			height = width * Game::GetWindowHeight() / Game::GetWindowWidth();
		}
		void SetHeight(float nheight)
		{
			// sets height, calculates wisth from aspect ratio of screen
			height = nheight;
			width = height * Game::GetWindowWidth() / Game::GetWindowHeight();
		}
		void SetPos(const Vec2& npos) { pos = npos; }
		const sf::View& GetView()
		{
			view.reset(sf::FloatRect(pos.x - width / 2, pos.y - height / 2, width, height));
			return view;
		}
	};
}