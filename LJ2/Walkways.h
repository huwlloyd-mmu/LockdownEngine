#pragma once
#include "LockdownEngine.h"
#include <vector>

class City;
class Walkways
{
public:
	// defines the graph of paths that people can walk on
	struct Vertex
	{
		LE::Vec2 pos;
		std::vector<int> edges; // incident edges
	};
	std::vector<Vertex> v; // the vertices of the graph
	struct Edge
	{
		int v0;
		int v1;
		int junction; 
	};
	std::vector<Edge> e; // the edges of the graph
public:
	Walkways(City* city);
	void DebugDraw();
	LE::Vec2 NearestPointOnWalkways(const LE::Vec2& p);

};