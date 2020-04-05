#pragma once
#include "GameObject.h"
namespace LE
{
	class Level
	{
		// a level is just a bunch of game objects
		std::vector<GameObject*> objectList;
	public:
		Level() {}
		void AddObject(GameObject* g) { objectList.push_back(g); }
		void DeleteObject(GameObject* g)
		{
			auto it = std::find(objectList.begin(), objectList.end(), g);
			if (it != objectList.end())
				objectList.erase(it);

		}
		std::vector<GameObject*>& GetObjectList() { return objectList; }
	};
}