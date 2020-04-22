#pragma once
#include "Vec2.h"
#include <vector>
#include <list>
#include <set>
class City;
class Vehicle;
class Pedestrian;

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
		
		// pedestrians using the junction
		std::set<Pedestrian*> peds;
		// the vehicle queueing system, works via a mutex with FIFO
		std::list<Vehicle*> fifo;
		bool Acquire(Vehicle* v); 		
		void Release();
		bool IsFree() { return fifo.size() == 0 || peds.size() > 0; }
		void AddPed(Pedestrian* ped) { peds.insert(ped); }
		void RemovePed(Pedestrian* ped) { peds.erase(ped); }
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