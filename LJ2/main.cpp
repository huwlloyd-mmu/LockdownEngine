#include "LockdownEngine.h"

void main()
{
	LE::Game::SetWindow(1024, 832, "CamTest", false);

	// create a level
	LE::Level* level0 = new LE::Level();
	LE::Game::AddLevel(level0);
	LE::GameObject obj;
	LE::Texture tex("data/test.png");
	obj.AddComponent(new LE::SpriteComponent(tex, 1.0f));
	level0->AddObject(&obj);
	obj.SetPosition(5.0f, 5.0f);
	obj.SetPivot(0.5f, 0.5f);

	LE::Game::GetCamera().SetWidth(4.0f);
	LE::Game::GetCamera().SetPos(LE::Vec2(7.0f, 5.0f));
	LE::Game::Run();
}