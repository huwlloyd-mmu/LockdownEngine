#pragma once
#include <SFML/Graphics.hpp>
#include "Texture.h"
#include <vector>

namespace LE
{
	class SpriteBatchComponent
	{
		sf::VertexArray vArray;
		unsigned int numSprites;
		sf::Texture* texture;
	public:
		// offsets are the bottom left corner of the sprite, in world and uv coords
		SpriteBatchComponent(const std::vector<Vec2>& worldOffset, const std::vector<Vec2>& worldSize, const std::vector<Texture>& textures)
			: vArray( sf::VertexArray(sf::PrimitiveType::Triangles) )
		{
			texture = textures[0].GetSFTexturePointer();
			numSprites = worldOffset.size();

			for (int i = 0; i < numSprites; i++)
			{
				// add a quad
				Vec2 uv0 = Vec2(textures[i].GetSFTextureRect().left, textures[i].GetSFTextureRect().top);
				Vec2 uv1 = Vec2(textures[i].GetSFTextureRect().width, textures[i].GetSFTextureRect().height);
				vArray.append(sf::Vertex(sf::Vector2f(worldOffset[i].x, worldOffset[i].y), sf::Vector2f(uv0.x, uv0.y)));
				vArray.append(sf::Vertex(sf::Vector2f(worldOffset[i].x, worldOffset[i].y+worldSize[i].y), sf::Vector2f(uv0.x, uv0.y+uv1.y)));
				vArray.append(sf::Vertex(sf::Vector2f(worldOffset[i].x + worldSize[i].x, worldOffset[i].y + worldSize[i].y), sf::Vector2f(uv0.x + uv1.x, uv0.y + uv1.y)));

				vArray.append(sf::Vertex(sf::Vector2f(worldOffset[i].x, worldOffset[i].y), sf::Vector2f(uv0.x, uv0.y)));
				vArray.append(sf::Vertex(sf::Vector2f(worldOffset[i].x + worldSize[i].x, worldOffset[i].y + worldSize[i].y), sf::Vector2f(uv0.x + uv1.x, uv0.y + uv1.y)));
				vArray.append(sf::Vertex(sf::Vector2f(worldOffset[i].x + worldSize[i].x, worldOffset[i].y), sf::Vector2f(uv0.x + uv1.x, uv0.y)));
			}
		}

		void Draw(sf::RenderWindow& window, const sf::Transform& t)
		{
			sf::RenderStates state;
			state.texture = texture;
			state.transform = t;
			window.draw(vArray, state);
		}
	};
}