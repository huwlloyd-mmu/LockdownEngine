#pragma once

#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"
#include "Texture.h"
#include "game.h"

namespace LE
{
	class CircleComponent : public Component
	{
		// draws a circle
		sf::CircleShape circle;
	public:
		CircleComponent(float radius, const sf::Color& fillColor, const sf::Color& outlineColor)
		{
			circle = sf::CircleShape(radius);
			circle.setFillColor(fillColor);
			circle.setOutlineColor(outlineColor);
			circle.setOrigin(sf::Vector2f(radius, radius));
		}

		virtual CircleComponent* Clone() const
		{
			return new CircleComponent(circle.getRadius(), circle.getFillColor(), circle.getOutlineColor());
		}

		virtual void Draw(sf::RenderWindow& window, const sf::Transform& transform)
		{
			window.draw(circle, transform);
		}
	};
}