#include "vehicle.h"
#include "city.h"
#include "roads.h"
#include "AnimatedSprite.h"

Vehicle::Vehicle( City * city )
{
	c = city;
	rng = std::mt19937(std::random_device()());

	std::uniform_int_distribution<int> dist(0, 47);
	obj = LE::Game::SpawnCopy(city->vehicleProtos[dist(rng)]);
	state = OnRoad;
	PlaceRandomly();

}

void Vehicle::UpdateOnRoad(float dt) 
{
	LE::Vec2 v0 = c->roads->roads[curRoad].v0;
	LE::Vec2 v1 = c->roads->roads[curRoad].v1;
	float len = (v1 - v0).magnitude();
	curDistance += dt * 2.0f;

	LE::Vec2 pos = v0 + (v1 - v0) * (curDistance / len);

	if (curDistance > len)
	{
		curDistance = len;
		// we hit a junction, start the process of turning
		curJunction = c->roads->roads[curRoad].j;
		// pick a road to take, no u-turns allowed
		// find which road this is
		int thisRoad = -1;
		for (int i = 0; i < 4; i++)
		{
			if (curRoad == c->roads->junctions[curJunction].roadsIn[i])
			{
				thisRoad = i;
				break;
			}
		}
		if (thisRoad == -1)
			std::cout << "shouldn't be here!" << std::endl;

		int forbiddenRoad = (thisRoad + 2) % 4;

		// choose a road out
		nextRoad = forbiddenRoad;
		std::uniform_int_distribution<int> dist(0, 3);
		while (nextRoad == forbiddenRoad || c->roads->junctions[curJunction].roadsOut[nextRoad] == -1)
			nextRoad = dist(rng);
		// start the turning process
		if (c->roads->junctions[curJunction].roadsOut[nextRoad] == -1)
			std::cout << "bad thing happened!" << std::endl;
		state = Turning1;
		curDistance = 0.0f;
		// work out the turning point
		int turns[4][4] = { {0,0,-1,3}, {0,1,1,-1}, {-1,1,2,2}, {3,-1,2,3} };
		turningPoint = turns[nextRoad][thisRoad];
		if ( turningPoint == -1 )
			std::cout << "shouldn't be here, either!" << std::endl;
		pos = v1;
	}
	pos = c->WorldToIso(pos) - LE::Vec2(0.0,0.75f);
	obj->SetPosition(pos);
}

void Vehicle::UpdateTurning1(float dt) 
{
	LE::Vec2 v0 = c->roads->roads[curRoad].v1;
	LE::Vec2 v1 = c->roads->junctions[curJunction].turnPoints[turningPoint];

	float len = (v1 - v0).magnitude();
	curDistance += dt * 1.0f;
	LE::Vec2 pos = v0 + (v1 - v0) * (curDistance / len);
	if (curDistance > len)
	{
		pos = v1;
		state = Turning2;
		curDistance = 0.0f;
	}
	pos = c->WorldToIso(pos) - LE::Vec2(0.0f,0.75f);
	obj->SetPosition(pos);

}

void Vehicle::UpdateTurning2(float dt)
{
	LE::Vec2 v0 = c->roads->junctions[curJunction].turnPoints[turningPoint];
	LE::Vec2 v1 = c->roads->roads[c->roads->junctions[curJunction].roadsOut[nextRoad]].v0;

	if (v0.y == v1.y)
	{
		if (v1.x < v0.x)
			obj->GetComponent<LE::AnimatedSpriteComponent>()->StartMode("left");
		else
			obj->GetComponent<LE::AnimatedSpriteComponent>()->StartMode("right");
	}
	else
	{
		if (v1.y < v0.y)
			obj->GetComponent<LE::AnimatedSpriteComponent>()->StartMode("down");
		else
			obj->GetComponent<LE::AnimatedSpriteComponent>()->StartMode("up");
	}

	float len = (v1 - v0).magnitude();
	curDistance += dt * 1.0f;
	LE::Vec2 pos = v0 + (v1 - v0) * (curDistance / len);
	if (curDistance > len)
	{
		pos = v1;
		state = OnRoad;
		curRoad = c->roads->junctions[curJunction].roadsOut[nextRoad];
		curDistance = 0;
	}
	pos = c->WorldToIso(pos) - LE::Vec2(0.0f,0.75f);
	obj->SetPosition(pos);
}
void Vehicle::UpdateWaiting(float dt)
{

}

void Vehicle::Update(float dt)
{
	// update the position of the graphics object
	LE::Vec2 v0 = c->roads->roads[curRoad].v0;
	LE::Vec2 v1 = c->roads->roads[curRoad].v1;
	float len = (v1 - v0 ).magnitude();

	// update the movement
	switch (state) // a bit lazy, should give the engine a generic state machine system
	{
	case OnRoad:
		UpdateOnRoad(dt);
		break;
	case Turning1:
		UpdateTurning1(dt);
		break;
	case Turning2:
		UpdateTurning2(dt);
		break;
	case Waiting:
		UpdateWaiting(dt);
		break;
	}
}
void Vehicle::PlaceRandomly()
{
	// pick a random road
	std::uniform_int_distribution<int> dist(0, c->roads->roads.size() - 1);
	curRoad = dist(rng);
	// pick a random distance
	LE::Vec2 v0, v1;
	v0 = c->roads->roads[curRoad].v0;
	v1 = c->roads->roads[curRoad].v1;
	float len = (v1 - v0).magnitude();
	std::uniform_real_distribution<float> fdist(0, len);
	curDistance = fdist(rng);
	if (v0.y == v1.y)
	{
		if ( v1.x < v0.x)
			obj->GetComponent<LE::AnimatedSpriteComponent>()->StartMode("left");
		else
			obj->GetComponent<LE::AnimatedSpriteComponent>()->StartMode("right");
	}
	else
	{ 
		if (v1.y < v0.y)
			obj->GetComponent<LE::AnimatedSpriteComponent>()->StartMode("down");
		else
			obj->GetComponent<LE::AnimatedSpriteComponent>()->StartMode("up");
	}

}