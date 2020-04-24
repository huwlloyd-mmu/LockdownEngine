#include "Pedestrian.h"
#include "Circle.h"
#include "city.h"
#include "AnimatedSprite.h"
#include "roads.h"

Pedestrian::Pedestrian(City* city) : city(city), rng(std::mt19937(std::random_device()()))
{
	// pick a random skin
	rng = std::mt19937(std::random_device()());

	std::uniform_int_distribution<int> dist(0, 24);
	obj = LE::Game::SpawnCopy(city->pedProto[dist(rng)]);

	highlight = new LE::GameObject();
	LE::Texture hlTex("data/highlight.png");
	LE::SpriteComponent* spr = new LE::SpriteComponent(hlTex, 2.0f);
	spr->SetClip();
	spr->SetSort();
	highlight->AddComponent(spr);
	LE::Game::AddToLevel(highlight);

	PlaceRandomly();
	obj->GetComponent<LE::AnimatedSpriteComponent>()->StartMode("stand_left");

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
	highlight->SetPosition(isoPos + LE::Vec2(-1.0f, -0.5f));
	highlight->GetComponent<LE::SpriteComponent>()->SetZ(isoPos.y + 0.9f);

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

//	if (fdist(rng) < edgeLength * 0.3f)
//		curState = STOPPED;
	UpdatePosition();
}

void Pedestrian::ChooseTurningRandom()
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
	// if we were on a junction, release it
	if (city->walkways->e[curEdge].junction != -1)
		city->roads->junctions[city->walkways->e[curEdge].junction].RemovePed(this); // deregister

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
}

void Pedestrian::ChooseTurningForTarget()
{

}

bool Pedestrian::MoveAlong(float dt)
{
	// move along the current segment. Return true if we hit the end
	// check if we need to wait at a junction
	if (city->walkways->e[curEdge].junction != -1)
		if (!(city->roads->junctions[city->walkways->e[curEdge].junction].IsFree()))
		{
			moving = false;
			return false; // wait
		}
		else
			city->roads->junctions[city->walkways->e[curEdge].junction].AddPed(this); // register

	// move along the current edge in the current direction
	float delta = 1.0f * dt;

	if (curDirection)
		curEdgeDist += delta;
	else
		curEdgeDist -= delta;
	// probably should cache this in the edge for convenience
	float edgeLength = (city->walkways->v[city->walkways->e[curEdge].v0].pos - city->walkways->v[city->walkways->e[curEdge].v1].pos).magnitude();
	if (curEdgeDist < 0.0f || curEdgeDist > edgeLength)
		return true;
	return false;
}
void Pedestrian::Update(float dt)
{
	moving = true;
	switch (curState)
	{
		case WALK_RANDOMLY:
			if (MoveAlong(dt))
				ChooseTurningRandom();
			break;
		case WALK_TO_TARGET:
			if (MoveAlong(dt))
				ChooseTurningForTarget();
			break;
		case STOPPED:
			moving = false;
			break;
	}
	// set the animation
		// figure out the direction
	int dir;

	if (city->walkways->v[city->walkways->e[curEdge].v0].pos.x == city->walkways->v[city->walkways->e[curEdge].v1].pos.x)
	{
		// y edge, so going up or down
		float yDiff;
		if (curDirection)
			yDiff = city->walkways->v[city->walkways->e[curEdge].v1].pos.y - city->walkways->v[city->walkways->e[curEdge].v0].pos.y;
		else
			yDiff = city->walkways->v[city->walkways->e[curEdge].v0].pos.y - city->walkways->v[city->walkways->e[curEdge].v1].pos.y;
		if (yDiff > 0.0)
			dir = 0;
		else
			dir = 1;
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
			dir = 2;
		else
			dir = 3;
	}

	if (moving)
	{
		if ( dir == 0)
			obj->GetComponent<LE::AnimatedSpriteComponent>()->StartMode("walk_up", true);
		else if (dir == 1)
			obj->GetComponent<LE::AnimatedSpriteComponent>()->StartMode("walk_down", true);
		else if (dir == 2)
			obj->GetComponent<LE::AnimatedSpriteComponent>()->StartMode("walk_right", true);
		else
			obj->GetComponent<LE::AnimatedSpriteComponent>()->StartMode("walk_left", true);
	}
	else
	{
		if (dir == 0)
			obj->GetComponent<LE::AnimatedSpriteComponent>()->StartMode("stand_up", true);
		else if (dir == 1)
			obj->GetComponent<LE::AnimatedSpriteComponent>()->StartMode("stand_down", true);
		else if (dir == 2)
			obj->GetComponent<LE::AnimatedSpriteComponent>()->StartMode("stand_right", true);
		else
			obj->GetComponent<LE::AnimatedSpriteComponent>()->StartMode("stand_left", true);
	}
	UpdatePosition();
}
