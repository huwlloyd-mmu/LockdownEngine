#include "roads.h"
#include "city.h"

Roads::Roads(City* c, int border)
{
	city = c;

	int nCellsX, nCellsY;
	int roadWidth = city->GetRoadWidth();
	int blockSizeX = city->GetBlockSizeX();
	int blockSizeY = city->GetBlockSizeY();
	nCellsX = city->GetNX() / blockSizeX - border * 2;
	nCellsY = city->GetNY() / blockSizeY - border * 2;

	// first create the junctions
	for (int i = 0; i < nCellsX+1; i++)
	{
		for (int j = 0; j < nCellsY+1; j++)
		{
			LE::Vec2 junctionPos = LE::Vec2((border + i) * blockSizeX + 1.0f, (border + j) * blockSizeY + 1.0f);
			Junction junction;
			for (int k = 0; k < 4; k++)
			{
				junction.roadsIn[k] = junction.roadsOut[k] = -1;
			}
			junction.centre = junctionPos;
			junction.turnPoints[2] = junctionPos + LE::Vec2(-0.5f, -0.5f);
			junction.turnPoints[3] = junctionPos + LE::Vec2(-0.5f, 0.5f);
			junction.turnPoints[0] = junctionPos + LE::Vec2(0.5f, 0.5f);
			junction.turnPoints[1] = junctionPos + LE::Vec2(0.5f, -0.5f);
			junctions.push_back(junction);
		}
	}
	// now add roads, hooking up to the junctions as we go
	int roadCount = 0;
	for (int i = 0; i < nCellsX+1; i++)
	{
		for (int j = 0; j < nCellsY+1; j++)
		{
			// at each i, j we create the roads going to the right and down from this junction (if any) 
			int ij;
			int ijRight;
			int ijDown;

			ij = i * (nCellsY + 1) + j;
			ijRight = ij + nCellsY + 1;
			ijDown = ij + 1;

			Road r;
			int iRoad;
			// horizontal roads
			if (i < nCellsX)
			{
				// left to right road
				r.v0 = junctions[ij].centre + LE::Vec2(1.5f, -0.5f);
				r.v1 = r.v0 + LE::Vec2(blockSizeX - 3.0f, 0.0f);
				r.j = ijRight;
				roads.push_back(r); 
				iRoad = roadCount++;
				junctions[ij].roadsOut[2] = junctions[ijRight].roadsIn[2] = iRoad;
				// right to left road
				r.v1 = junctions[ij].centre + LE::Vec2(1.5f, 0.5f);
				r.v0 = r.v1 + LE::Vec2(blockSizeX - 3.0f, 0.0f);
				r.j = ij;
				roads.push_back(r);
				iRoad = roadCount++;
				junctions[ij].roadsIn[0] = junctions[ijRight].roadsOut[0] = iRoad;
			}
			// vertical roads
			if (j < nCellsY)
			{
				// downwards road
				r.v0 = junctions[ij].centre + LE::Vec2(0.5f, 1.5f);
				r.v1 = r.v0 + LE::Vec2(0.0f, blockSizeY - 3.0f);
				r.j = ijDown;
				roads.push_back(r);
				iRoad = roadCount++;
				junctions[ij].roadsOut[1] = junctions[ijDown].roadsIn[1] = iRoad;
				// upwards road
				r.v1 = junctions[ij].centre + LE::Vec2(-0.5f, 1.5f);
				r.v0 = r.v1 + LE::Vec2(0.0, blockSizeY - 3.0f);
				r.j = ij;
				roads.push_back(r);
				iRoad = roadCount++;
				junctions[ij].roadsIn[3] = junctions[ijDown].roadsOut[3] = iRoad;
			}
		}
	}
}

bool Roads::Junction::Acquire(Vehicle* v)
{
	// is this thing already in the queue?
	if (std::find(fifo.begin(), fifo.end(), v) == fifo.end())
	{
		// if not add it at the back
		fifo.push_back(v);
	}
	return v == *fifo.begin() && peds.size() == 0;
}

void Roads::Junction::Release()
{
	// pop the first in the queue
	fifo.pop_front();
}
