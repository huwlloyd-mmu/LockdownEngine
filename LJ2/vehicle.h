#pragma once
#include <random>
#include "GameObject.h"
#include "Circle.h"

class City;

class Vehicle
{
	// a car that drives around the roads
	City* c;
	int curRoad; 
	int nextRoad;
	float curDistance;
	int curJunction;
	int turningPoint;

	std::mt19937 rng;
	enum State
	{
		OnRoad,
		Turning1,
		Turning2,
	};
	State state;
	LE::GameObject* obj;
	// state updaters
	void UpdateOnRoad(float dt);
	void UpdateTurning1(float dt);
	void UpdateTurning2(float dt);
public:
	Vehicle(City* city);
	void PlaceRandomly();
	void Update(float dt);
};