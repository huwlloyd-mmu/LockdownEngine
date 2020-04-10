#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"
#include "Texture.h"
#include "game.h"

namespace LE
{
	class SpriteComponent : public Component
	{
		sf::Sprite sprite;
		float worldToPixelScale;
		float worldSize;
		sf::Vector2f centre;
		SpriteComponent() {}

		void setupSprite( const Texture &tex )
		{
			sprite.setTexture(tex.GetSFTexture());
			sprite.setTextureRect(tex.GetSFTextureRect());
			// calculate the scaling to go from world to pixels
			// we equate worldSize to the larger of the width and height
			float w, h, size;
			w = tex.GetWidth();
			h = tex.GetHeight();
			size = w;
			if (h > size) size = h;
			worldToPixelScale = worldSize / size;
			centre = sf::Vector2f(w * 0.5f, h * 0.5f);
		}
	public:
		virtual SpriteComponent* Clone() const
		{
			SpriteComponent* newSprite = new SpriteComponent();
			newSprite->worldSize = worldSize;
			newSprite->worldToPixelScale = worldToPixelScale;
			newSprite->centre = centre;
			newSprite->sprite.setTexture(*(sprite.getTexture()));
			newSprite->sprite.setTextureRect(sprite.getTextureRect());
			return newSprite;
		}
		bool XFlip(bool flip) 
		{ 
			sf::IntRect tr = sprite.getTextureRect();
			tr.width *= -1;
			sprite.setTextureRect(tr);
		}
		bool YFlip(bool flip) 
		{ 
			sf::IntRect tr = sprite.getTextureRect();
			tr.height *= -1;
			sprite.setTextureRect(tr);
		}

		SpriteComponent(const Texture& tex, float nWorldSize )
		{
			worldSize = nWorldSize;
			setupSprite(tex);
		}
		virtual void Draw(sf::RenderWindow& window, const sf::Transform &transform)
		{
			sf::Transform t = transform;
			t.scale(sf::Vector2f(worldToPixelScale, worldToPixelScale));
			window.draw(sprite, t);
		}

		void SetTexture(const Texture& t) { setupSprite(t); }
	};
}