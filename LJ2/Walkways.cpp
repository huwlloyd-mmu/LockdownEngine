#include "Walkways.h"
#include "City.h"

Walkways::Walkways(City* city)
{
	// first create all the vertices.
	const static float inset = 0.2f; // distance in grid units that walkway is offset from the road
	int border = 3; // number of city blocks around the outside of town that are non-navigable

	int nCellsX, nCellsY;
	int roadWidth = city->GetRoadWidth();
	int blockSizeX = city->GetBlockSizeX();
	int blockSizeY = city->GetBlockSizeY();
	nCellsX = city->GetNX() / blockSizeX - border*2;
	nCellsY = city->GetNY() / blockSizeY - border*2;

	// vertices
	for (int i = 0; i < nCellsX; i++)
	{
		for (int j = 0; j < nCellsY; j++)
		{
			float x, y;
			Vertex vert;
			// four vertices for each cell
			x = (border + i) * blockSizeX + roadWidth + inset;
			y = (border + j) * blockSizeY + roadWidth + inset;
			vert.pos = LE::Vec2(x, y);
			v.push_back(vert);

			x = (border + i) * blockSizeX + roadWidth + inset;
			y = (border + j + 1) * blockSizeY - inset;
			vert.pos = LE::Vec2(x, y);
			v.push_back(vert);

			x = (border + i+1) * blockSizeX  - inset;
			y = (border + j+1) * blockSizeY  - inset;
			vert.pos = LE::Vec2(x, y);
			v.push_back(vert);

			x = (border + i+1) * blockSizeX - inset;
			y = (border + j) * blockSizeY + roadWidth + inset;
			vert.pos = LE::Vec2(x, y);
			v.push_back(vert);

		}
	}

	// now make the cells themselves
	int edgeCount = 0;
	int vCount = 0;
	for (int i = 0; i < nCellsX; i++)
	{
		for (int j = 0; j < nCellsY; j++)
		{
			int v0, v1, v2, v3;
			v0 = vCount++;
			v1 = vCount++;
			v2 = vCount++;
			v3 = vCount++;

			Edge edge;

			edge.v0 = v0; edge.v1 = v1;
			e.push_back(edge);
			v[v0].edges.push_back(edgeCount);
			v[v1].edges.push_back(edgeCount);
			edgeCount++;

			edge.v0 = v1; edge.v1 = v2;
			e.push_back(edge);
			v[v1].edges.push_back(edgeCount);
			v[v2].edges.push_back(edgeCount);
			edgeCount++;

			edge.v0 = v2; edge.v1 = v3;
			e.push_back(edge);
			v[v2].edges.push_back(edgeCount);
			v[v3].edges.push_back(edgeCount);
			edgeCount++;

			edge.v0 = v3; edge.v1 = v0;
			e.push_back(edge);
			v[v3].edges.push_back(edgeCount);
			v[v0].edges.push_back(edgeCount);
			edgeCount++;

		}
	}
	// add the crossings
	for (int i = 0; i < nCellsX; i++)
	{
		for (int j = 0; j < nCellsY; j++)
		{
			if (j > 0)
			{
				// connect bottom left to top left of cell below
			}
		}
	}
}

void Walkways::DebugDraw()
{

}