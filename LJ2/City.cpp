#include "City.h"
#include "LockdownEngine.h"
#include "IsoMap.h"

int City::GetTextureForTile( int i, int j)
{
	// used for building the base map, to figure out the road textures etc.
	bool here, above, below, left, right;
	here = tiles[i][j] == 1 || tiles[i][j] == 2;
	below = j > 0 && tiles[i][j - 1] == 1;
	above = j < ny - 1 && tiles[i][j + 1] == 1;
	left = i > 0 && tiles[i - 1][j] == 1;
	right = i < nx - 1 && tiles[i + 1][j] == 1;

	if (!here)
	{
		// not a road tile, but maybe a pavement tile
		if (below)
		{
			if (!right && !left)
				return 2;
			else if (right)
				return 12;
			else
				return 4;
		}
		if (above)
		{
			if (!right && !left)
				return 2;
			else if (right)
				return 27;
			else
				return 20;
		}
		return 2;
	}
	else
	{
		// is this a junction tile?
		if (tiles[i][j] == 2)
			return 8;
		// figure out what sort of road tile
		
		// first check junctions
		bool jbelow = j > 0 && tiles[i][j - 1] == 2;
		bool jabove = j < ny - 1 && tiles[i][j + 1] == 2;
		bool jleft = i > 0 && tiles[i - 1][j] == 2;
		bool jright = i < nx - 1 && tiles[i + 1][j] == 2;

		if (jabove && !left)
			return 22;
		if (jabove && !right)
			return 14;
		if (jbelow && !left ) 
			return 23;
		if (jbelow && !right)
			return 15;

		if (jleft && !above)
			return 30;
		if (jleft && !below)
			return 7;
		if (jright && !above)
			return 29;
		if (jright && !below)
			return 6;

		if (below && !above)
			return 16;
		if (above && !below)
			return 24;
		if (left && !right)
			return 1;
		if (right && !left)
			return 9;
	}
}


City::City()
{
	// initialize the tiles
	for (int i = 0; i < nx; i++)
	{
		std::vector<unsigned int> column;
		for (int j = 0; j < ny; j++)
		{
			column.push_back(0);
		}
		tiles.push_back(column);
	}
	// make some roads
	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < ny; j++)
		{
			if (i % 16 > 13 || j % 12 > 9)
			{
				tiles[i][j] = 1; // regular road
			}
			if (i % 16 > 13 && j % 12 > 9)
			{
				tiles[i][j] = 2; // junction
			}

		}
	}

	// now make the tilemap
	LE::GameObject* obj = new LE::GameObject();;
	isoMap =  new LE::IsoMapComponent(nx, ny, 1.0f);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			LE::Texture tex("data/econU_tiles.png", LE::Vec2(i * 0.25f, j * 0.125f), LE::Vec2(0.25f, 0.125f));
			isoMap->AddTile(tex);
		}
	}
	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < ny; j++)
		{
			isoMap->SetCell(i, j, GetTextureForTile(i, j));
		}
	}
	isoMap->CreateSprites();
	obj->AddComponent(isoMap);
	LE::Game::AddToLevel(obj);
}