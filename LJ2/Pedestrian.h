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

public:
	Pedestrian(City* city); 
	void PlaceRandomly();
	void Update(float dt);

};