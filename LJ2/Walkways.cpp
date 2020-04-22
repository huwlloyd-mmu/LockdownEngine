#include "Walkways.h"
#include "City.h"

Walkways::Walkways(City* city, int border)
{
	// border is the number of city blocks around the outside of town that are non-navigable
	// first create all the vertices.
	const static float inset = 0.2f; // distance in grid units that walkway is offset from the road

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
			LE::Vec2 pos[4];
			// six vertices for each cell
			x = (border + i) * blockSizeX + roadWidth + inset;
			y = (border + j) * blockSizeY + roadWidth + inset;
			vert.pos = LE::Vec2(x, y);
			pos[0] = vert.pos;
			v.push_back(vert);

			x = (border + i) * blockSizeX + roadWidth + inset;
			y = (border + j + 1) * blockSizeY - inset;
			vert.pos = LE::Vec2(x, y);
			pos[1] = vert.pos;
			v.push_back(vert);

			x = (border + i+1) * blockSizeX  - inset;
			y = (border + j+1) * blockSizeY  - inset;
			vert.pos = LE::Vec2(x, y);
			pos[2] = vert.pos;
			v.push_back(vert);

			x = (border + i+1) * blockSizeX - inset;
			y = (border + j) * blockSizeY + roadWidth + inset;
			vert.pos = LE::Vec2(x, y);
			pos[3] = vert.pos;
			v.push_back(vert);

			vert.pos = (pos[1] + pos[2])*0.5f;
			v.push_back(vert);

			vert.pos = (pos[0] + pos[3]) * 0.5f;
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
			int v0, v1, v2, v3, v4, v5;
			v0 = vCount++;
			v1 = vCount++;
			v2 = vCount++;
			v3 = vCount++;
			v4 = vCount++;
			v5 = vCount++;
			Edge edge;

			edge.v0 = v0; edge.v1 = v1; edge.junction = -1;
			e.push_back(edge);
			v[v0].edges.push_back(edgeCount);
			v[v1].edges.push_back(edgeCount);
			edgeCount++;

			edge.v0 = v1; edge.v1 = v4; edge.junction = -1;
			e.push_back(edge);
			v[v1].edges.push_back(edgeCount);
			v[v4].edges.push_back(edgeCount);
			edgeCount++;

			edge.v0 = v4; edge.v1 = v2; edge.junction = -1;
			e.push_back(edge);
			v[v4].edges.push_back(edgeCount);
			v[v2].edges.push_back(edgeCount);
			edgeCount++;

			edge.v0 = v2; edge.v1 = v3; edge.junction = -1;
			e.push_back(edge);
			v[v2].edges.push_back(edgeCount);
			v[v3].edges.push_back(edgeCount);
			edgeCount++;

			edge.v0 = v3; edge.v1 = v5; edge.junction = -1;
			e.push_back(edge);
			v[v3].edges.push_back(edgeCount);
			v[v5].edges.push_back(edgeCount);
			edgeCount++;

			edge.v0 = v5; edge.v1 = v0; edge.junction = -1;
			e.push_back(edge);
			v[v5].edges.push_back(edgeCount);
			v[v0].edges.push_back(edgeCount);
			edgeCount++;

			edge.v0 = v4; edge.v1 = v5; edge.junction = -1;
			e.push_back(edge);
			v[v4].edges.push_back(edgeCount);
			v[v5].edges.push_back(edgeCount);
			edgeCount++;
		}
	}
	// add the crossings
	for (int i = 0; i < nCellsX; i++)
	{
		for (int j = 0; j < nCellsY; j++)
		{
			Edge edge;

			// connect bottom left and bottom right to top left and top right of cell below
			if (j > 0)
			{
				int v0, v1; // bottom left and bottom right vertices of this cell
				int u0, u1; // top left and right vertices of cell below
				v0 = (i * nCellsY + j)*6 + 0;
				v1 = (i * nCellsY + j)*6 + 3;
				u0 = (i * nCellsY + j - 1 )*6 + 1;
				u1 = (i * nCellsY + j - 1 )*6 + 2;

				int ij0 = i * (nCellsY + 1) + j;
				int ij1 = (i+1) * (nCellsY + 1) + j;


				edge.v0 = v0; edge.v1 = u0; edge.junction = ij0;
				e.push_back(edge);
				v[v0].edges.push_back(edgeCount);
				v[u0].edges.push_back(edgeCount);
				edgeCount++;

				edge.v0 = v1; edge.v1 = u1; edge.junction = ij1;
				e.push_back(edge);
				v[v1].edges.push_back(edgeCount);
				v[u1].edges.push_back(edgeCount);
				edgeCount++;
			}
			// connect top right and bottom right to top left and bottom left of cell to the right
			if (i < nCellsX - 1)
			{
				int v0, v1; // top right and bottom right vertices of this cell
				int u0, u1; // top left and bottom left vertices of cell to the right
				v0 = (i * nCellsY + j) * 6 + 2;
				v1 = (i * nCellsY + j) * 6 + 3;
				u0 = ((i +1 ) * nCellsY + j) * 6 + 1;
				u1 = ((i +1 ) * nCellsY + j) * 6 + 0;

				int ij0 = (i + 1)* (nCellsY + 1) + j+1;
				int ij1 = (i + 1) * (nCellsY + 1) + j;

				edge.v0 = v0; edge.v1 = u0; edge.junction = ij0;
				e.push_back(edge);
				v[v0].edges.push_back(edgeCount);
				v[u0].edges.push_back(edgeCount);
				edgeCount++;

				edge.v0 = v1; edge.v1 = u1; edge.junction = ij1;
				e.push_back(edge);
				v[v1].edges.push_back(edgeCount);
				v[u1].edges.push_back(edgeCount);
				edgeCount++;
			}
		}
	}
}

void Walkways::DebugDraw()
{

}