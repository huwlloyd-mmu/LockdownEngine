#pragma once
class City;

class ShadowGame
{
	// the main game class
	City* city;
public:
	~ShadowGame() {};
	ShadowGame() {};
	void Run();
};