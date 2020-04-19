#include "City.h"
#include "LockdownEngine.h"
#include "IsoMap.h"
#include "Walkways.h"
#include "AnimatedSprite.h"
#include "DbgPosition.h"

void CityUpdater::Update(float dt)
{
	city->Update(dt);
}

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
			if ( ((i % blockSizeX) < roadWidth) || ((j % blockSizeY) < roadWidth) )
			{
				tiles[i][j] = 1; // regular road
			}
			if ( ((i % blockSizeX) < roadWidth) && ((j % blockSizeY) < roadWidth))
			{
				tiles[i][j] = 2; // junction
			}
		}
	}

	// now make the tilemap
	LE::GameObject* obj = new LE::GameObject();
	//new DebugPosition(this);
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
	walkways = new Walkways(this);
	MakePedProtos();

	// add some pedestrians
	for (int i = 0; i < 1000; i++)
	{
		peds.push_back(new Pedestrian(this));
	}
	// add the updater game object
	updater = new LE::GameObject();
	updater->AddComponent(new CityUpdater(this));
	LE::Game::AddToLevel(updater);

	// add buildings
	PlaceBuildings();

}

void City::Update(float dt)
{
	for (auto p : peds)
		p->Update(dt);
}

void City::MakePedProtos()
{
	for (int j = 0; j < 25; j++)
	{
		// load the textures
		LE::Texture* animFrames[36];
		for (int i = 0; i < 36; i++)
		{
			int xOffset = i + i / 9; // i/9 is to skip the sitting animation
			animFrames[i] = new LE::Texture("data/people.png", LE::Vec2((float)xOffset * (32.0f / 1376.0f), j*(1.0f/25.0f)), LE::Vec2(32.0f / 1376.0f, 1.0f / 25.0f));
		}
		LE::AnimatedSpriteComponent* sc = new LE::AnimatedSpriteComponent(1.0f);

		sc->SetClip();
		sc->SetSort();
		std::vector<LE::Texture*> mode;
		std::string names[8] = { "stand_up", "stand_right", "stand_left", "stand_down",
							"walk_up", "walk_right", "walk_left", "walk_down" };

		for (int i = 0; i < 4; i++)
		{
			// add the stand mode
			mode.push_back(animFrames[i * 9]);
			LE::Animation* anim = new LE::Animation(mode, 15.0f, true);
			sc->AddMode(names[i], anim);
			mode.clear();
			for (int j = 0; j < 8; j++)
				mode.push_back(animFrames[i * 9 + j]);
			anim = new LE::Animation(mode, 15.0f, true);
			sc->AddMode(names[i + 4], anim);
		}
		pedProto[j] = new LE::GameObject();
		pedProto[j]->AddComponent(sc);
	}
}

void City::PlaceBuildings()
{
	// for now, we'll have one building type

	float x0 = 256;
	float x1 = 512;
	float y0 = 0;
	float y1 = 160;

	LE::Texture* tile = new LE::Texture("data/econU_buildings_1.png", LE::Vec2(x0 / 1280.0, y1 / 1280.0), LE::Vec2(256.0 / 1280.0, (y1-y0) / 1280.0f));

	int buildingSize = blockSizeY - 4;
	for (int x = 3; x < 256; x += blockSizeX)
	{
		for (int y = 3; y < 256; y += blockSizeY)
		{
			for (int xo = 0; xo < 2; xo++)
			{
				// place a building 
				Vec2 centre = Vec2(x+xo*(buildingSize+2), y) + Vec2(buildingSize, buildingSize) * 0.5f;
				Vec2 isoPos = isoMap->MapToIso(centre);
				float width = buildingSize * 2.0f;
				float height = width * (y1 - y0) / (x1 - x0);
				float yOffset = ((y1 - y0) / (x1 - x0) - 0.5f) * buildingSize;
				Vec2 spriteCentre = isoPos - Vec2(0, yOffset);
				spriteCentre -= Vec2(width * 0.5, height * 0.5);
				SpriteComponent* sc = new SpriteComponent(*tile, width);
				sc->SetClip();
				sc->SetSort();
				sc->SetZ(isoPos.y);
				GameObject* obj = new GameObject();
				obj->SetPosition(spriteCentre);
				obj->AddComponent(sc);
				Game::AddToLevel(obj);
			}
		}
	}
}