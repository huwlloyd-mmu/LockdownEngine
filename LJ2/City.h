#pragma once
#include <vector>
#include "IsoMap.h"
class Walkways;
class City
{
	// a procedural isometric city
	const int nx = 256;
	const int ny = 256;
	LE::IsoMapComponent* isoMap;
	std::vector<std::vector<unsigned int>> tiles;
	int GetTextureForTile(int i, int j);
	const int blockSizeX = 16;
	const int blockSizeY = 12;
	const int roadWidth = 2;
public:
	Walkways* walkways;
	City();
	LE::Vec2 WorldToIso(const LE::Vec2& pos) { return isoMap->MapToIso(pos); }
	LE::Vec2 IsoToWorld(const LE::Vec2& pos) { return isoMap->IsoToMap(pos); }
	int GetBlockSizeX() const { return blockSizeX;  }
	int GetBlockSizeY() const { return blockSizeY; }
	int GetRoadWidth() const { return roadWidth; }
	int GetNX() const { return nx; }
	int GetNY() const { return ny; }
};