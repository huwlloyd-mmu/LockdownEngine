#pragma once
#include "GameObject.h"
#include "Component.h"
#include "SpriteComponent.h"
#include "Texture.h"
#include <vector>

namespace LE
{
	class IsoMapComponent : public Component
	{
		// an isometric map. For now, it's a static map

		int nx, ny; // dimensions of the map
		float cellSize; // world space size of a cell
		std::vector<Texture> tiles;
		int *map; // the map, tile indices for cells i*ny + j
		std::vector<SpriteComponent> sprites;
		std::vector<sf::Vector2f> spriteOffsets;

	public:
		IsoMapComponent() {}
		IsoMapComponent(int nx, int ny, float cellSize) : nx(nx), ny(ny), cellSize(cellSize), map(new int[nx*ny]) 
		{
			// flood fill to -1 by default
			std::memset(map, 0xFF, nx * ny * sizeof(unsigned int));
		}
		unsigned int AddTile(const Texture& tex)
		{
			unsigned int index = tiles.size();
			tiles.push_back(tex);
			return index;
		}
		void SetCell(unsigned int i, unsigned int j, unsigned int tileIndex) { map[i * ny + j] = tileIndex; }
		Vec2 MapToIso(const Vec2& map) const 
		{ 
			return Vec2(map.x - map.y, (map.x + map.y) * 0.5f);
		}
		Vec2 IsoToMap(const Vec2& iso) const 
		{
			return Vec2( iso.y + iso.x * 0.5f, iso.y - iso.x * 0.5f );
		}
		void CreateSprites()
		{
			std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
			// bottom triangle, i.e. all diagonals starting at the left hand edge
			for (int j = 0; j < ny; j++)
			{
				int ii = 0;
				int jj = j;
				while (ii <= j && ii < nx)
				{
					if (map[ii * ny + jj] != 0xFFFFFFFF)
					{
						SpriteComponent sc(tiles[map[ii * ny + jj]], cellSize * 2);
						sprites.push_back(sc);
						Vec2 worldOffset = MapToIso(Vec2(cellSize * (ii + 0.5f), cellSize * (jj + 0.5f)));
						spriteOffsets.push_back(sf::Vector2f(worldOffset.x, worldOffset.y));
					}
					++ii; --jj;
				}
			}
			// top triangle, i.e. all diagonals starting at the top edge
			for (int i = 0; i < nx; i++)
			{
				int ii = i;
				int jj = ny-1;
				while (jj >= 0 && ii < nx)
				{
					if (map[ii * ny + jj] != 0xFFFFFFFF)
					{
						SpriteComponent sc(tiles[map[ii * ny + jj]], cellSize * 2);
						sprites.push_back(sc);
						Vec2 worldOffset = MapToIso(Vec2(cellSize * (ii + 0.5f), cellSize * (jj + 0.5f)));
						spriteOffsets.push_back(sf::Vector2f(worldOffset.x, worldOffset.y));
					}
					++ii; --jj;
				}
			}

		}

		virtual Component* Clone() const { return nullptr; }// you ain't gonna need it 
		virtual void Draw(sf::RenderWindow& window, const sf::Transform& transform)
		{
			for (int i = 0; i < sprites.size(); i++)
			{
				sf::Transform t = transform;
				t.translate(spriteOffsets[i]);
				sprites[i].Draw(window, t);
			}
		}



	};
}