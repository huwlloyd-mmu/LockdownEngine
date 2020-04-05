#pragma once

namespace LE
{
	class GameObject;
	class Component
	{
	public:
		GameObject* parent;
		// pure virtual copy function for cloning objects
		virtual Component* Clone() const = 0;

		void SetParent(GameObject* p) { parent = p; }
		virtual void Init() {}
		virtual void Update(float dt) {};
		virtual void Draw( sf::RenderWindow& window, const sf::Transform& transform) {}
	};
};