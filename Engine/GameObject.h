#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Component.h"
#include "Game.h"
#include "Vec2.h"

namespace LE
{
	class GameObject : private sf::Transformable
	{
		// we inherit from sf::Transformable to get the transform API, but do it privately
		// so we can wrap it with our own API 

		// a game object is a list of components, and the transform which it inherits from Transformable
		std::vector<Component*> components;
		Vec2 pos;
		Vec2 scale;
		Vec2 pivot;
		float rot;
		bool active = true;
		bool ui = false; // ui objects are rendered in screen space, and drawn after everything else
		std::string name = "default";
	public:
		GameObject() {}
		~GameObject()
		{
			for (auto c : components)
			{
				delete c;
			}
		}
		void AddComponent(Component* c) { components.push_back(c); c->SetParent(this); }
		void RemoveComponent(Component* c) 
		{ 
			auto it = std::find(components.begin(), components.end(), c);
			if (it != components.end())
				components.erase(it);
		}
		void Update(float dt)
		{
			if (!active)
				return;
			for (auto c : components)
				if ( c->active )
					c->Update(dt);
		}
		void Draw(sf::RenderWindow& window ) const
		{
			if (!active)
				return;
			for (auto c : components)
				if ( c->visible )
					c->Draw(window, getTransform());
		}
		void SetUI(bool nui) { ui = nui; } 
		const std::vector<Component*>& GetComponents() const { return components; }
		void SetPosition(const Vec2& npos) { SetPosition(npos.x, npos.y); }
		void SetPosition(float x, float y) { pos = Vec2(x, y); setPosition(sf::Vector2f(x, y)); }
		void SetRotation(float angle) { rot = angle;  setRotation(angle); }
		void SetScale(float sx, float sy) { scale = Vec2(sx, sy);  setScale(sf::Vector2f(sx, sy)); }
		void SetPivot(float x, float y) { pivot = Vec2(x, y);  setOrigin(sf::Vector2f(x, y)); }
		const Vec2& GetPosition() const { return pos;  }
		const Vec2& GetScale()  const { return scale; }
		float GetRotation()  const { return rot;  }
		void SetActive(bool bNewActive) { active = bNewActive; }
		void SetName(const std::string& newName) { name = newName; }
		const std::string& GetName() const { return name; }
		bool IsUI() const { return ui; }
		GameObject *Clone()
		{
			// spawn a copy and add it to the current level of the game
			GameObject* o = new GameObject();
			o->SetName(name + "clone");
			for (auto c : components)
			{
				o->AddComponent(c->Clone());
			}
			return o;
		}
		template<class T> T* GetComponent()
		{
			for (auto c : components)
			{
				if ( typeid(T) == typeid(*c))
					return (T*) c;
			}
			return nullptr;
		}
	};
}