#include "Pedestrian.h"
#include "Circle.h"
#include "city.h"
#include "AnimatedSprite.h"

Pedestrian::Pedestrian(City* city) : city(city), rng(std::mt19937(std::random_device()()))
{
	/*
	obj = new LE::GameObject();
	obj->AddComponent(new LE::CircleComponent(0.5f, sf::Color(255, 0, 0, 255), sf::Color(255, 0, 0, 255)));
	*/
	// pick a random skin
	rng = std::mt19937(std::random_device()());

	std::uniform_int_distribution<int> dist(0, 24);
	obj = LE::Game::SpawnCopy(city->pedProto[dist(rng)]);

	PlaceRandomly();
	obj->SetPosition(isoPos + LE::Vec2(0.0f,1.0f)); // offset so feet are on position
	obj->GetComponent<LE::AnimatedSpriteComponent>()->StartMode("stand_left");
	//LE::Game::AddToLevel(obj);
}

void Pedestrian::UpdatePosition()
{
	LE::Vec2 v0, v1;
	// 0 to 1
	v0 = city->walkways->v[city->walkways->e[curEdge].v0].pos;
	v1 = city->walkways->v[city->walkways->e[curEdge].v1].pos;
	float edgeLength = (v1 - v0).magnitude();
	pos = v0 + (v1 - v0) * (curEdgeDist / edgeLength);
	isoPos = city->WorldToIso(pos);
	obj->GetComponent<LE::AnimatedSpriteComponent>()->SetZ(isoPos.y+1.0f);
	obj->SetPosition(isoPos - LE::Vec2(0.0f,1.0f));
}

void Pedestrian::PlaceRandomly()
{
	// place on a random edge
	int numEdges = city->walkways->e.size();
	std::uniform_int_distribution<int> dist(0, numEdges - 1);
	curEdge = dist(rng);
	float edgeLength = (city->walkways->v[city->walkways->e[curEdge].v0].pos - city->walkways->v[city->walkways->e[curEdge].v1].pos).magnitude();
	// pick a random direction and distance along the edge
	std::uniform_real_distribution<float> fdist(0.0f, edgeLength);
	// use the same distribution for both tasks, for convenience only
	float val = fdist(rng);
	if (val < edgeLength * 0.5f)
		curDirection = 1;
	else
		curDirection = 0;
	curEdgeDist = fdist(rng);
	UpdatePosition();
}

void Pedestrian::Update(float dt)
{
	// move along the current edge in the current direction
	float delta = 1.0f * dt; 

	if (curDirection)
		curEdgeDist += delta;
	else
		curEdgeDist -= delta;
	// probably should cache this in the edge for convenience
	float edgeLength = (city->walkways->v[city->walkways->e[curEdge].v0].pos - city->walkways->v[city->walkways->e[curEdge].v1].pos).magnitude();
	if (curEdgeDist < 0.0f || curEdgeDist > edgeLength)
	{ 
		// choose a new edge
		int curVertex;
		if (curEdgeDist < 0.0f)
			curVertex = city->walkways->e[curEdge].v0;
		else
			curVertex = city->walkways->e[curEdge].v1;
		std::uniform_int_distribution<int> dist(0, city->walkways->v[curVertex].edges.size() - 1);
		int newEdge;
		do
		{
			newEdge = city->walkways->v[curVertex].edges[dist(rng)];
		} while (newEdge == curEdge);
		curEdge = newEdge;
		if (curVertex == city->walkways->e[curEdge].v0)
		{
			curDirection = 1;
			curEdgeDist = 0.0f;
		}
		else
		{
			curDirection = 0;
			curEdgeDist = (city->walkways->v[city->walkways->e[curEdge].v0].pos - city->walkways->v[city->walkways->e[curEdge].v1].pos).magnitude();
		}
		// figure out the direction
		if (city->walkways->v[city->walkways->e[curEdge].v0].pos.x == city->walkways->v[city->walkways->e[curEdge].v1].pos.x)
		{
			// y edge, so going up or down
			float yDiff;
			if (curDirection)
				yDiff = city->walkways->v[city->walkways->e[curEdge].v1].pos.y - city->walkways->v[city->walkways->e[curEdge].v0].pos.y;
			else
				yDiff = city->walkways->v[city->walkways->e[curEdge].v0].pos.y - city->walkways->v[city->walkways->e[curEdge].v1].pos.y;
			if (yDiff > 0.0)
				obj->GetComponent<LE::AnimatedSpriteComponent>()->StartMode("walk_up", true);
			else
				obj->GetComponent<LE::AnimatedSpriteComponent>()->StartMode("walk_down", true);
		}
		else
		{
			// x edge, so going right or left
			float xDiff;
			if (curDirection)
				xDiff = city->walkways->v[city->walkways->e[curEdge].v1].pos.x - city->walkways->v[city->walkways->e[curEdge].v0].pos.x;
			else
				xDiff = city->walkways->v[city->walkways->e[curEdge].v0].pos.x - city->walkways->v[city->walkways->e[curEdge].v1].pos.x;
			if (xDiff > 0.0)
				obj->GetComponent<LE::AnimatedSpriteComponent>()->StartMode("walk_right", true);
			else
				obj->GetComponent<LE::AnimatedSpriteComponent>()->StartMode("walk_left", true);
		}


	}
	UpdatePosition();
}
