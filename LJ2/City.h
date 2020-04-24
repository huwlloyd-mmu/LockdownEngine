#pragma once
#include <vector>
#include "IsoMap.h"
#include "Pedestrian.h"
#include "vehicle.h"

class Walkways;
class Roads;
class City; // forward dec
class CityUpdater : public LE::Component
{
	City* city;
public:
	CityUpdater(City* city) : city(city) {}
	virtual void Update(float dt);
	virtual CityUpdater* Clone() const { return nullptr; }
};

constexpr float vehicleSpriteSize = 1.8f;

class City
{
	// a procedural isometric city
	LE::IsoMapComponent* isoMap;
	LE::IsoMapComponent* isoMapBuildings;
	std::vector<std::vector<unsigned int>> tiles;
	int GetTextureForTile(int i, int j);
	const int blockSizeX = 22;
	const int blockSizeY = 12;
	const int roadWidth = 2;
	const int bordery = 4;
	const int borderx = 2;
	const int nx = 256 + blockSizeX * borderx * 2;
	const int ny = 256 + blockSizeY * bordery * 2;;
	LE::GameObject* updater;
	// pedestrians
	std::vector<Pedestrian*> peds;
	void AddPedestrians();
	void MakePedProtos(); // helper function to create pedProto
	// vehicles
	std::vector<Vehicle*> vehicles;
	void PlaceBuildings();
	void MakeVehicleProtos();
public:
	Walkways* walkways;
	Roads* roads;

	LE::GameObject* pedProto[25]; // prototype game objects for a pedestrian
	LE::GameObject* vehicleProtos[48]; // prototype game objects for vehicles
	City();
	LE::Vec2 WorldToIso(const LE::Vec2& pos) { return isoMap->MapToIso(pos); }
	LE::Vec2 IsoToWorld(const LE::Vec2& pos) { return isoMap->IsoToMap(pos); }
	int GetBlockSizeX() const { return blockSizeX;  }
	int GetBlockSizeY() const { return blockSizeY; }
	int GetRoadWidth() const { return roadWidth; }
	int GetBorderX() const { return borderx; }
	int GetBorderY() const { return bordery; }
	int GetNX() const { return nx; }
	int GetNY() const { return ny; }
	void Update(float dt);
};