#pragma once
#include <SFML/Graphics.hpp>
#include "game.h"

namespace LE
{
	class Texture
	{
		sf::Texture *texture;
		sf::IntRect textureRect;
	public:
		Texture(){}
		Texture(const std::string& name, const Vec2& origin = Vec2(0, 0), const Vec2& size = Vec2(1.0f, 1.0f))
		{
			texture = Game::Assets().GetTexture(name);
			sf::Vector2u tsize = texture->getSize();
			textureRect.left = tsize.x * origin.x;
			textureRect.top = tsize.y * origin.y;
			textureRect.width = size.x * tsize.x;
			textureRect.height = size.y * tsize.y;
		}
		const sf::Texture& GetSFTexture() const { return *texture;  }
		const sf::IntRect& GetSFTextureRect() const { return textureRect;  }
		float GetWidth() const { return textureRect.width; } 
		float GetHeight() const { return textureRect.height; }
	};
}