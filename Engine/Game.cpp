#include <iostream>
#include "game.h"
#include "camera.h"

using namespace LE;

std::vector<Level*> Game::levels;
int Game::curLevel;
Vec2 Game::mousePos;

WindowInfo Game::windowInfo;
AssetManager Game::assets;
Timer Game::timer;
int Game::mbBuffer;
bool Game::mouseButtons[2][MouseButtons::MB_COUNT];
DebugPrint Game::dbg;
Camera Game::camera;

void Game::ClearInputs()
{
	mouseButtons[0][MouseButtons::MB_LEFT] = mouseButtons[1][MouseButtons::MB_LEFT] = 0;
	mouseButtons[0][MouseButtons::MB_RIGHT] = mouseButtons[1][MouseButtons::MB_RIGHT] = 0;
	mouseButtons[0][MouseButtons::MB_MIDDLE] = mouseButtons[1][MouseButtons::MB_MIDDLE] = 0;
}

void Game::UpdateInputs()
{
	mbBuffer ^= 1;
	mouseButtons[mbBuffer][MouseButtons::MB_LEFT] = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
	mouseButtons[mbBuffer][MouseButtons::MB_RIGHT] = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
	mouseButtons[mbBuffer][MouseButtons::MB_MIDDLE] = sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle);
	sf::Vector2i pos = sf::Mouse::getPosition(windowInfo.GetSFMLWindow());
	// clamp mouse to window
	bool clamped = false;
	if (pos.x < 0)
	{
		pos.x = 0; clamped = true;
	}
	if (pos.y < 0)
	{
		pos.y = 0; clamped = true;
	}
	if (pos.x > windowInfo.GetSFMLWindow().getSize().x-1)
	{
		pos.x = windowInfo.GetSFMLWindow().getSize().x-1; clamped = true;
	}
	if (pos.y > windowInfo.GetSFMLWindow().getSize().y-1)
	{
		pos.y = windowInfo.GetSFMLWindow().getSize().y-1; clamped = true;
	}
#if 0
	if (clamped)
		sf::Mouse::setPosition(pos, windowInfo.GetSFMLWindow() );
#endif
	mousePos = Vec2(pos.x, pos.y);
}

void Game::SetWindow(int width, int height, const std::string& title, bool fullscreen )
{
	windowInfo.SetWidth(width);
	windowInfo.SetHeight(height);
	windowInfo.SetTitle(title);
	windowInfo.SetFullscreen(fullscreen);
}

void Game::Update(float dt)
{
	UpdateInputs();
	for (auto o : levels[curLevel]->GetObjectList())
		o->Update(dt);
}


void Game::Draw(sf::RenderWindow &window )
{
	for (auto o : levels[curLevel]->GetObjectList())
		o->Draw(window);
	// draw the debug text
	//dbg.Draw();
}

GameObject* Game::FindObject(const std::string& name)
{
	Level* level = levels[curLevel];
	for (auto o : level->GetObjectList())
		if (o->GetName() == name)
			return o;
	return nullptr;
}

void Game::Run()
{
	windowInfo.Create();
	dbg.SetWindow(&windowInfo.GetSFMLWindow());

	// main loop
	// Start the game loop
	sf::RenderWindow &window = windowInfo.GetSFMLWindow();
	float time = 0.0f;
	timer.Reset();
	ClearInputs();
	while (window.isOpen())
	{
		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window: exit
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					window.close();
				}
			}
		}
		float newTime = timer.Elapsed();
		Update(newTime-time);
		time = newTime;
		// Clear screen
		window.clear();
		// Update the window
		window.setView(camera.GetView());
		Draw(window);
		window.display();
	}


	timer.Reset();
}