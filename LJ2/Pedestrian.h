#pragma once
#include "Walkways.h"
#include <random>
#include "LockdownEngine.h"

class Pedestrian
{
	// a pedstrian, who walks around the walkways
	City* city;

	int curEdge;
	int curDirection;
	float curEdgeDist;
	std::mt19937 rng; // random number generator
	LE::Vec2 pos;
	LE::Vec2 isoPos;
	void UpdatePosition(); // helper with works out position from edge data

	LE::GameObject* obj;
	LE::GameObject* highlight; // really, this should be a child object of obj. I may refactor if I get a chance.

	LE::Vec2 target;  // a point on a walkway to aim for.
	enum State
	{
		STOPPED,
		WALK_RANDOMLY,
		WALK_TO_TARGET
	};
	bool moving = false;
	State curState = WALK_RANDOMLY;
	void ChooseTurningRandom();
	void ChooseTurningForTarget();
	bool MoveAlong(float dt);
	bool bHighlighted = false;
public:
	Pedestrian(City* city);
	void PlaceRandomly();
	void Update(float dt);
	void Highlight(bool on) { bHighlighted = on; highlight->SetActive(bHighlighted); }
	LE::Vec2 GetPos() const { return pos; }
	LE::Vec2 GetIsoPos() const { return isoPos; }
	void Stop() { curState = STOPPED; }
	void SetTarget(const LE::Vec2& pos);
};