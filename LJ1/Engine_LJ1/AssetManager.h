#pragma once
#include <map>
#include <iostream>
#include "sound.h"

namespace LE
{
	class AssetManager
	{
		std::map<std::string, sf::Texture*> textures;
		std::map<std::string, sf::Font*> fonts;
		std::map<std::string, Sound*> sounds;
	public:

		Sound* GetSound(const std::string& name)
		{
			if (sounds.find(name) == sounds.end())
			{
				// load the sound
				Sound* sound = new Sound(name);
				sounds[name] = sound;
			}
			return sounds[name];
		}

		sf::Font* GetFont(const std::string& name)
		{
			if (fonts.find(name) == fonts.end())
			{
				// load the texture
				sf::Font* font = new sf::Font();
				if (font->loadFromFile(name))
				{
					fonts[name] = font;
				}
				else
				{
					std::cerr << "could not load font " << name << std::endl;
					return font;
				}
			}
			return fonts[name];

		}
		sf::Texture* GetTexture(const std::string& name)
		{
			if (textures.find(name) == textures.end())
			{
				// load the texture
				sf::Texture* tex = new sf::Texture();;
				if (tex->loadFromFile(name))
				{
					textures[name] = tex;
				}
				else
				{
					std::cerr << "could not load texture " << name << std::endl;
					return tex;
				}
			}
			return textures[name];
		}
	};
}