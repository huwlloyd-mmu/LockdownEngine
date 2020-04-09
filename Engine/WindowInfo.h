#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class WindowInfo
{
	int width, height;
	std::string title;
	bool fullscreen;
	sf::RenderWindow *window;
public:
	WindowInfo() : width(640), height(480), title("LockdownEngine"), fullscreen(false) {}
	void SetWidth(int nWidth) { width = nWidth; }
	void SetHeight(int nHeight) { height = nHeight; }
	void SetTitle(const std::string& nTitle) { title = nTitle; }
	void SetFullscreen(bool bFull) { fullscreen = bFull; }
	float GetWidth() const { return width; }
	float GetHeight() const { return height; }
	void Create() 
	{ 
		window = new sf::RenderWindow(sf::VideoMode(width, height), title, fullscreen ? sf::Style::Fullscreen : sf::Style::Default); 
		window->setVerticalSyncEnabled(true);
	}

	 sf::RenderWindow& GetSFMLWindow() const { return *window; }
};