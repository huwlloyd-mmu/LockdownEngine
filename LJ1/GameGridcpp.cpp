#include "GameGrid.h"
#include "Gem.h"

GameGrid::GameGrid()
{
	// load some textures
	colTex[0] = LE::Texture("data/gems/blue/gem1/0001.png");
	colTex[1] = LE::Texture("data/gems/green/gem2/0001.png");
	colTex[2] = LE::Texture("data/gems/pink/gem3/0001.png");
	colTex[3] = LE::Texture("data/gems/yellow/gem4/0001.png");
	colTex[4] = LE::Texture("data/gems/green/gem5/0001.png");

	GameObject gemProto;

	gameProto.AddComponent(new SpriteComponent(colTex[0], 32.0f));
	gameProto.AddComponent(new Gem());

	grid = new GameObject * [nx];

	for (int i = 0; i < nx; i++)
	{
		grid[i] = new GameObject[ny];

		for (int j = 0; j < ny; j++)
		{
			grid[i][j] = gameProto.Clone();
			//grid[i][j].
		}
	}
}