#include "LockdownEngine.h"
#include "GameGrid.h"

int main()
{
	LE::Game::SetWindow(1024, 832, "2048Buster", false);

	// create a level
	LE::Level *level0 = new LE::Level();
	LE::Game::AddLevel(level0);
	LE::GameObject obj;
	obj.AddComponent(new GameGrid(16,12,1024,832));
	level0->AddObject(&obj);
	obj.SetPosition(100, 100);
	obj.SetPivot(50, 50);
	
	// for now will rely on the order I add objects to the level for z sorting...
	LE::GameObject scoreText;
	scoreText.SetName("ScoreText");
	TextComponent* t = new TextComponent("data/gomarice_round_pop.ttf", 50);
	scoreText.AddComponent(t);
	t->SetColor(sf::Color(255, 255, 255, 255));
	t->SetText("Score");
	scoreText.SetPosition(0, 0); // why -50? Need to figure this out
	level0->AddObject(&scoreText);

	// game over text
	LE::GameObject gameOverText;
	t = new TextComponent("data/gomarice_round_pop.ttf", 100);
	gameOverText.AddComponent(t);
	t->SetColor(sf::Color(255, 255, 255, 255));
	t->SetText("Game Over");
	gameOverText.SetPosition(220, 300); // why -50? Need to figure this out
	gameOverText.SetName("GameOver");
	gameOverText.SetActive(false);
	level0->AddObject(&gameOverText);

	LE::GameObject clickText;
	t = new TextComponent("data/gomarice_round_pop.ttf", 50);
	clickText.AddComponent(t);
	t->SetColor(sf::Color(255, 255, 255, 255));
	t->SetText("Click to Retry");
	clickText.SetPosition(300, 400); // why -50? Need to figure this out
	clickText.SetName("Click");
	clickText.SetActive(false);
	level0->AddObject(&clickText);

	LE::Music* music = new LE::Music("data/gamemusic.ogg");
	music->Play(true);
	LE::Game::Run();
}