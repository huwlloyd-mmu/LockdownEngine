#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"
#include "Texture.h"
#include "game.h"

namespace LE
{
	class TextComponent : public Component
	{
		int charSize;
		sf::Text text;
		std::string fontName;
	public:
		virtual TextComponent* Clone() const
		{
			TextComponent* newText = new TextComponent(fontName, charSize);
			return newText;
		}
		TextComponent(std::string fontName, int charSize) : fontName(fontName), charSize(charSize)
		{
			text.setFont(*Game::Assets().GetFont(fontName));
			text.setCharacterSize(charSize);
		}
		virtual void Draw(sf::RenderWindow& window, const sf::Transform& transform)
		{
			sf::Transform t = transform;	
			window.draw(text, t);
		}
		void SetText(const std::string& t) { text.setString(t); }
		void SetColor(const sf::Color& color) { text.setFillColor(color); text.setOutlineColor(color); }
	};
}