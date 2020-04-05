#pragma once
#include "Game.h"
#include "Component.h"
#include "Texture.h"
#include "gem.h"
#include <random>
// main class for the bubble buster game

using namespace LE;

class GameGrid : public Component
{
	int nx;
	int ny;
	int pixPerBall;
	int yStart;

	const static int numColors = 8;
	const static int numPowers = 11;
	Texture colTex[numColors];
	Texture powerTex[numPowers];
	std::mt19937 rng; // mersenne twister RNG
	LE::GameObject* gemProto;
	float goTime; // time since game over
	struct Cell
	{
		GameObject* obj;
		Gem* gem; // cached, so we don't need to keep calling GetComponent
		int color; 
		// other stuff...
	};
	Cell** grid;
	void HighlightAll(int i, int j, int color);
	std::vector<std::pair<int,int>> highlights;
	void DeleteCell(int i, int j);
	void DropAndShuffle();
	bool CheckGameOver();
	int NeighbourColor(int i, int j);
	void CalcScore(int& score, int& multiplier);
	void StartGame();
	void ClearGame();
public:
	GameGrid() {}
	GameGrid( int nnx, int nny, int screenW, int screenH );

	virtual void Update(float dt);
	virtual void Draw() {};
	virtual GameGrid* Clone() const { return new GameGrid(); }
};

