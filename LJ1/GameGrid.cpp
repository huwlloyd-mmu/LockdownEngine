#include "GameGrid.h"
#include "Texture.h"
#include "SpriteComponent.h"
#include "TextComponent.h"
#include "Gem.h"

void GameGrid::StartGame()
{
	std::uniform_int_distribution<int> dist(0, 3);
	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < ny; j++)
		{
			grid[i][j].obj = Game::SpawnCopy(gemProto);
			grid[i][j].obj->SetPivot(pixPerBall / 2, pixPerBall / 2);
			grid[i][j].obj->SetPosition(i * pixPerBall + pixPerBall / 2, j * pixPerBall + pixPerBall / 2 + yStart);
			grid[i][j].gem = grid[i][j].obj->GetComponent<Gem>();
			grid[i][j].color = dist(rng);
			grid[i][j].gem->SetTextures(colTex[grid[i][j].color], colTex[7]);
			grid[i][j].gem->SetTarget(i * pixPerBall + pixPerBall / 2, j * pixPerBall + pixPerBall / 2 + yStart);
		}
	}
}

void GameGrid::ClearGame()
{
	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < ny; j++)
		{
			if (grid[i][j].obj != nullptr)
				DeleteCell(i, j);
		}
	}
}

GameGrid::GameGrid(int nnx, int nny, int screenW, int screenH )
{
	nx = nnx;
	ny = nny;
	pixPerBall = screenW / nx;
	yStart = pixPerBall;

	// rng
	rng = std::mt19937(std::random_device()());
	// load some textures

	// GameArtForge - billiard balls (opengameart)
	colTex[0] = LE::Texture("data/all.png", Vec2(0.75f, 0.00f), Vec2(0.25f, 0.25f));
	colTex[1] = LE::Texture("data/all.png", Vec2(0.75f, 0.25f), Vec2(0.25f, 0.25f));
	colTex[2] = LE::Texture("data/all.png", Vec2(0.75f, 0.50f), Vec2(0.25f, 0.25f));
	colTex[3] = LE::Texture("data/all.png", Vec2(0.75f, 0.75f), Vec2(0.25f, 0.25f));
	// powers of 2
	powerTex[0] = LE::Texture("data/all.png", Vec2(0.0f, 0.00f), Vec2( 0.25f, 0.25f));
	powerTex[1] = LE::Texture("data/all.png", Vec2(0.0f, 0.25f), Vec2( 0.25f, 0.25f));
	powerTex[2] = LE::Texture("data/all.png", Vec2(0.0f, 0.50f), Vec2( 0.25f, 0.25f));
	powerTex[3] = LE::Texture("data/all.png", Vec2(0.0f, 0.75f), Vec2( 0.25f, 0.25f));
	powerTex[4] = LE::Texture("data/all.png", Vec2(0.25f, 0.00f), Vec2( 0.25f, 0.25f));
	powerTex[5] = LE::Texture("data/all.png", Vec2(0.25f, 0.25f), Vec2( 0.25f, 0.25f));
	powerTex[6] = LE::Texture("data/all.png", Vec2(0.25f, 0.50f), Vec2( 0.25f, 0.25f));
	powerTex[7] = LE::Texture("data/all.png", Vec2(0.25f, 0.75f), Vec2( 0.25f, 0.25f));
	powerTex[8] = LE::Texture("data/all.png", Vec2(0.5f, 0.00f), Vec2(0.25f, 0.25f));
	powerTex[9] = LE::Texture("data/all.png", Vec2(0.5f, 0.25f), Vec2(0.25f, 0.25f));
	powerTex[10] = LE::Texture("data/all.png", Vec2(0.5f, 0.5f), Vec2(0.25f, 0.25f));


	gemProto = new LE::GameObject();

	gemProto->AddComponent(new LE::SpriteComponent(colTex[0], pixPerBall));
	gemProto->AddComponent(new Gem());
	gemProto->GetComponent<Gem>()->SetTextures(colTex[0], colTex[1]);

	grid = new Cell *[nx];
	std::uniform_int_distribution<int> dist(0, 3);
	for (int i = 0; i < nx; i++)
	{
		grid[i] = new Cell[ny];
	}
	StartGame();
}

int GameGrid::NeighbourColor(int i, int j)
{
	// helper function
	if (i < 0 || i > nx - 1 || j < 0 || j > ny - 1 || grid[i][j].obj == nullptr)
		return -1000;
	else
		return grid[i][j].color;
}

bool GameGrid::CheckGameOver()
{
	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < ny; j++)
		{
			if (grid[i][j].obj != nullptr)
			{
				int color = grid[i][j].color;
				if (color == NeighbourColor(i - 1, j)) return false;
				if (color == NeighbourColor(i + 1, j)) return false;
				if (color == NeighbourColor(i, j - 1)) return false;
				if (color == NeighbourColor(i, j + 1)) return false;
			}
		}
	}
	return true;
}

void GameGrid::Update( float dt)
{
	Vec2 mousePos = Game::GetMousePos();
	// update the score
	int score, mult;
	CalcScore(score, mult);
	std::string scoreString;
	scoreString = "Score: " + std::to_string(score) + " Mult: x" +   std::to_string(mult);
	Game::FindObject("ScoreText")->GetComponent<TextComponent>()->SetText(scoreString);
	// first check whether game is over
	if (CheckGameOver())
	{
		goTime += dt;
		// if so, show the game over message
		GameObject* o = Game::FindObject("GameOver");
		if (o != nullptr)
			o->SetActive(true);
		for (int i = 0; i < nx; i++)
		{
			for (int j = 0; j < ny; j++)
			{
				if (grid[i][j].obj != nullptr)
				{
					grid[i][j].gem->Highlight(false);
				}
			}
		}
		if (goTime > 1.0f)
		{
			o = Game::FindObject("Click");
			o->SetActive(true);
			float s = sin(goTime * 0.8f);
			s *= s;
			o->GetComponent<TextComponent>()->SetColor(sf::Color(255, 255, 255, 255 * s));
			if (Game::MouseClicked(MouseButtons::MB_LEFT))
			{
				o->SetActive(false);
				o = Game::FindObject("GameOver");
				o->SetActive(false);
				ClearGame();
				StartGame();
			}
		}
		return;
	}
	goTime = 0.0f;
	// clear highlights
	bool anyMoving = false;
	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < ny; j++)
		{
			if (grid[i][j].obj != nullptr)
			{
				grid[i][j].gem->Highlight(false);
				if (grid[i][j].gem->Moving())
					anyMoving = true;
			}
		}
	}
	// highlight the gem that the mouse is over
	int ig, jg;
	ig = mousePos.x / pixPerBall;
	jg = (mousePos.y - yStart)/ pixPerBall;
	highlights.clear();
	if ( grid[ig][jg].obj != nullptr && !anyMoving && mousePos.y >= yStart)	
		HighlightAll(ig, jg, grid[ig][jg].color);

	// check for mouse clicked, and delete all the highlights if there is more than one
	if (highlights.size() > 1)
	{
		if (Game::MouseClicked(MouseButtons::MB_LEFT))
		{
			Game::Assets().GetSound("data/bubblepop.ogg")->Play(false);
			// find the power of two.
			int cur = 256;
			int power = 8;
			int num = highlights.size();
			while ( (num&cur) == 0)
			{
				cur /= 2;
				power--;
			}
			int startPower = 0;
			if (grid[ig][jg].color < 0)
				startPower = -grid[ig][jg].color;
			power--;
			power += startPower;
			grid[ig][jg].gem->SetTextures(powerTex[power], powerTex[power]);
			grid[ig][jg].color = -power - 1;
			for (int i = 1; i < highlights.size(); i++ )
			{
				auto hl = highlights[i];
				DeleteCell(hl.first, hl.second);
			}
			DropAndShuffle();
		}
	}
	else if (highlights.size() == 1)
	{
		// clear the highlight
		grid[highlights[0].first][highlights[0].second].gem->Highlight(false);
	}
}

void GameGrid::HighlightAll(int ih, int jh,int color)
{
	if (ih < 0 || ih > nx - 1 || jh < 0 || jh > ny - 1 ||
		grid[ih][jh].color != color || grid[ih][jh].obj == nullptr || grid[ih][jh].gem->GetHighlight()) // will need null check here
		return;
	grid[ih][jh].gem->Highlight(true);
	highlights.push_back(std::pair<int, int>(ih, jh));
	// highlight neighbours
	HighlightAll(ih - 1, jh , color);
	HighlightAll(ih + 1, jh, color);
	HighlightAll(ih, jh - 1, color);
	HighlightAll(ih, jh + 1, color);
}

void GameGrid::DeleteCell(int ic, int jc)
{
	// remove object from the level
	Game::DestroyObject(grid[ic][jc].obj);
	grid[ic][jc].obj = nullptr;
}

void GameGrid::CalcScore(int& score, int& multiplier)
{
	int totNum = 0;
	int totBall = 0;
	int maxNum = 0;
	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < ny; j++)
		{
			if (grid[i][j].obj != nullptr)
			{
				if (grid[i][j].color < 0)
				{
					int power = -grid[i][j].color;
					if (maxNum < power)
						maxNum = power;
					totNum += 1 << power;
				}
				else
					++totBall;
			}
		}
	}
	int mults[] = { 1,2,3,5,8,12,20,30,40,50,60,70,80,90,100 };
	multiplier = mults[maxNum];
	score = maxNum * multiplier - totBall;
}

void GameGrid::DropAndShuffle()
{
	for (int i = 0; i < nx; i++)
	{
		for (int j = ny - 1; j >= 0; j--)
		{
			if (grid[i][j].obj != nullptr)
			{
				int jc = j;
				while (grid[i][jc + 1].obj == nullptr && jc < ny-1)
				{
					jc++;
				}
				if (j != jc)
				{
					grid[i][jc] = grid[i][j];
					grid[i][jc].gem->SetTarget(i * pixPerBall + pixPerBall/2, jc * pixPerBall + pixPerBall/2 + yStart);
					grid[i][j].obj = nullptr;
				}
			}
		}
	}
	// shuffle along
	for (int i = nx-1; i > 0; i--)
	{
		if (grid[i][ny - 1].obj == nullptr)
		{
			// this column is empty
			bool someLeft = false;
			for (int ic = i; ic > 0; ic--)
			{
				if (grid[ic][ny - 1].obj != nullptr)
					someLeft = true;
				for (int j = 0; j < ny; j++)
				{
					grid[ic][j] = grid[ic - 1][j];
					if ( grid[ic][j].obj != nullptr)
						grid[ic][j].gem->SetTarget(ic * pixPerBall + pixPerBall/2, j * pixPerBall + pixPerBall/2 + yStart);
				}
			}
			// remove the first column
			for (int j = 0; j < ny; j++)
				grid[0][j].obj = nullptr;
			if (someLeft)
				i++;
		}
	}
}