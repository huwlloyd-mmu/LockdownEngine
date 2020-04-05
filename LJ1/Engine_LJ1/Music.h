#pragma once
#include <SFML/Audio.hpp>
#include <string>

namespace LE
{
	class Music
	{
		sf::Music music;
	public:
		Music(const std::string& file)
		{
			music.openFromFile(file);
		}
		void Play(bool loop)
		{
			music.setLoop(loop);
			music.play();
		}
	};
}