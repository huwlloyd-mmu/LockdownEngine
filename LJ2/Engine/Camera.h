#pragma once
#include <SFML/Graphics.hpp>
#include "Vec2.h"
#include "Game.h"

namespace LE
{
	class Camera
	{
		Vec2 pos = Vec2(10.0f, 10.0f);
		float width = 20.0f;
		float height = 15.0f;
		sf::View view;
		float damping = 0.0f;
		Vec2 target = Vec2(10.0f, 10.0f);
	public:
		Camera() {};
		void Update(float deltaT)
		{
			float interpFac = 1.0f - pow(damping, deltaT * 60.0f); // correct to 60 fps
			pos = pos * interpFac + target * (1.0f - interpFac);
		}
		void SetWidth(float nwidth)
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
		void BlendTo(const Vec2& npos, float ndamping = 0.01f) { damping = ndamping; target = npos; }
		const sf::View& GetView()
		{
			view.reset(sf::FloatRect(pos.x - width / 2, pos.y - height / 2, width, height));
			return view;
		}
		sf::FloatRect GetRect()
		{
			sf::FloatRect rect;
			rect.left = pos.x - width / 2;
			rect.top = pos.y - height / 2;
			rect.width = width;
			rect.height = height;
			return rect;
		}

		float GetWidth() const { return width; }
		float GetHeight() const { return height; }
		const Vec2& GetPos() const { return pos; }
	};
};