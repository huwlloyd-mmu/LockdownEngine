#include "ShadowGame.h"
#include "LockdownEngine.h"
#include "IsoMap.h"
#include "city.h"
#include "DbgPosition.h"
#include <random>

using namespace LE;

void ShadowGame::Run()
{
	LE::Game::SetWindow(1024, 768, "TilesTest", false);

	// create a level
	LE::Level* level0 = new LE::Level();
	LE::Game::AddLevel(level0);

	city = new City();
	new DebugPosition(city);

	// add a co-ordinate tracker
	LE::Game::GetCamera().SetWidth(128.0f);
	LE::Game::GetCamera().SetPos(city->WorldToIso(LE::Vec2(128.0f, 128.0f)));
	LE::Game::Run();
}