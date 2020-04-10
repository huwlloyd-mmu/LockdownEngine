#include "ShadowGame.h"
#include "LockdownEngine.h"
#include "IsoMap.h"
#include <random>

using namespace LE;

void ShadowGame::Run()
{
	LE::Game::SetWindow(1024, 768, "TilesTest", false);

	// create a level
	LE::Level* level0 = new LE::Level();
	LE::Game::AddLevel(level0);

	LE::GameObject obj;
	LE::IsoMapComponent iso(16, 16, 1.0f);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			LE::Texture tex("data/econU_tiles.png", Vec2(i*0.25f, j*0.125f), Vec2(0.25f, 0.125f) );
			iso.AddTile(tex);
		}
	}
	std::mt19937 rng;
	std::uniform_int_distribution<unsigned int> dist(0, 31);
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			iso.SetCell(i, j, dist(rng));
		}
	}
	iso.CreateSprites();
	obj.AddComponent(&iso);
	level0->AddObject(&obj);

	LE::Game::GetCamera().SetWidth(32.0f);
	LE::Game::GetCamera().SetPos(LE::Vec2(0.0f, 8.0f));
	LE::Game::Run();
}