#pragma once
#include <SFML/Audio.hpp>

namespace LE
{
	class Sound
	{
		sf::SoundBuffer soundBuffer;
		sf::Sound sound;
	public:
		Sound(const std::string& fileName)
		{
			soundBuffer.loadFromFile(fileName);
			sound.setBuffer(soundBuffer);
		}
		void Play(bool loop)
		{
			sound.setLoop(loop);
			sound.play();
		}
	};
}