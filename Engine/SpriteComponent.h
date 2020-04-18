#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"
#include "Texture.h"
#include "game.h"
#include "camera.h"
#include <cinttypes>

namespace LE
{
	constexpr uint32_t SpriteFlagsSort = 1;
	constexpr uint32_t SpriteFlagsClip = 2;

	class SpriteComponent : public Component
	{
		sf::Sprite sprite;
		float worldToPixelScale;
		float worldSize;
		sf::Vector2f centre;

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
		int spriteFlags;
		SpriteComponent() : spriteFlags(0) {}
		virtual SpriteComponent* Clone() const
		{
			SpriteComponent* newSprite = new SpriteComponent();
			newSprite->worldSize = worldSize;
			newSprite->worldToPixelScale = worldToPixelScale;
			newSprite->centre = centre;
			newSprite->sprite.setTexture(*(sprite.getTexture()));
			newSprite->sprite.setTextureRect(sprite.getTextureRect());
			newSprite->spriteFlags = spriteFlags;
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

		SpriteComponent(float nWorldSize) : spriteFlags(0)
		{
			// this version of the constructor used by AnimatedSpriteComponent - no texture at the time of invocation
			worldSize = nWorldSize;
		}

		SpriteComponent(const Texture& tex, float nWorldSize ) : spriteFlags(0)
		{
			worldSize = nWorldSize;
			setupSprite(tex);
		}

		void SetClip() { spriteFlags |= SpriteFlagsClip; }
		void SetNoClip() { spriteFlags &= ~SpriteFlagsClip; }
		void SetSort() { spriteFlags |= SpriteFlagsSort; }
		void SetNoSort() { spriteFlags &= ~SpriteFlagsSort; }

		virtual void Draw(sf::RenderWindow& window, const sf::Transform &transform)
		{
			sf::Transform t = transform;
			t.scale(sf::Vector2f(worldToPixelScale, worldToPixelScale));
			// clip against the screen
			if (spriteFlags & SpriteFlagsClip)
			{
				sf::FloatRect boundingBox = sprite.getGlobalBounds();
				sf::FloatRect camRect = Game::GetCamera().GetRect();
				boundingBox = t.transformRect(boundingBox);
				if (boundingBox.left > camRect.left + camRect.width || boundingBox.left + boundingBox.width < camRect.left ||
					boundingBox.top > camRect.top + camRect.height || boundingBox.top + boundingBox.height < camRect.top)
				{
					// clip
					return;
				}
			}
			window.draw(sprite, t);
		}

		void SetTexture(const Texture& t) { setupSprite(t); }
	};
}