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
		Waiting
	};
	State state;
	LE::GameObject* obj;
	// state updaters
	void UpdateOnRoad(float dt);
	void UpdateTurning1(float dt);
	void UpdateTurning2(float dt);
	void UpdateWaiting(float dt);
public:
	Vehicle(City *city) : c(city)
	{
		rng = std::mt19937(std::random_device()());
		PlaceRandomly();
		obj = new LE::GameObject();
		obj->AddComponent(new LE::CircleComponent(0.5f, sf::Color(255, 0, 255, 255), sf::Color(255, 0, 255, 255)));
		LE::Game::AddToLevel(obj);
		state = OnRoad;
	}
	void PlaceRandomly();
	void Update(float dt);
};