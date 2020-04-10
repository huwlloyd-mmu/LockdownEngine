#pragma once
#include <vector>

class City
{
	// a procedural isometric city
	const int nx = 256;
	const int ny = 256;

	std::vector<std::vector<unsigned int>> tiles;
	int GetTextureForTile(int i, int j);
public:
	City();
};