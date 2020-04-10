#pragma once
#include <vector>
#include "IsoMap.h"
class City
{
	// a procedural isometric city
	const int nx = 256;
	const int ny = 256;
	LE::IsoMapComponent* isoMap;
	std::vector<std::vector<unsigned int>> tiles;
	int GetTextureForTile(int i, int j);
public:
	City();
	LE::Vec2 WorldToIso(const LE::Vec2& pos) { return isoMap->MapToIso(pos); }
};