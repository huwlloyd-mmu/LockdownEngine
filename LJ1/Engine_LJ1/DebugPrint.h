#pragma once
#include <string>
#include <list>
#include <SFML/Graphics.hpp>

namespace LE
{
	class DebugPrint
	{
		sf::Font font;
		const int fontSize = 30;
		const int lifetime = 2000;
		struct ConsoleLine
		{
			int age;
			std::string text;
			ConsoleLine() {}
			ConsoleLine(const std::string& text) : text(text), age(0) {}
		};
		std::list<ConsoleLine> console;
		sf::RenderWindow* window;
	public:
		DebugPrint()
		{
			// load font here
			font.loadFromFile("data/monogram.ttf");
		}
		void SetWindow(sf::RenderWindow* w) { window = w; }
		void AddLine(const std::string& line)
		{
			console.push_back(ConsoleLine(line));
		}
		void Draw()
		{
			//run through, calculating size 
			auto it = console.begin();
			int lines = 0;
			int maxWidth = 0;
			while (it != console.end())
			{
				it->age++;
				if (it->age > lifetime)
					it = console.erase(it);
				else
				{
					++lines;
					if (it->text.size() > maxWidth)
						maxWidth = it->text.size();
					++it;
				}
			}
			sf::Text t;
			t.setFont(font);
			t.setFillColor(sf::Color(255, 0, 0, 255));
			t.setCharacterSize(fontSize);
			it = console.begin();
			int x = 10;
			int y = 10;
			while (it != console.end())
			{
				t.setString(it->text);
				t.setPosition(sf::Vector2f(x, y));
				window->draw(t);
				y += fontSize;
				++it;
			}
		}
		

	};
}