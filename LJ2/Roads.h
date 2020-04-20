#pragma once
#include "Vec2.h"
#include <vector>
class City;

class Roads
{
	City* city;
public:
	struct Junction
	{
		LE::Vec2 centre;
		int roadsIn[4];
		int roadsOut[4];
		LE::Vec2 turnPoints[4]; // turning points for the various roads out
	};

	struct Road
	{
		LE::Vec2 v0, v1;
		int j; // index of the junction at the end of this road segment.
	};

	std::vector<Junction> junctions;
	std::vector<Road> roads;
	Roads(City* c, int border);
};