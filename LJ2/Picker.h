#pragma once
#include "LockdownEngine.h"
#include "City.h"
using namespace LE;

class PickerBehaviour : public Component
{
	TextComponent* text = nullptr;
	City* c;
	enum Mode
	{
		PickPed,
		PickDest
	};
	Mode curMode = PickPed;
	int selectedPed;
	void UpdateSelectPed();
	void UpdateSelectDest();
	SpriteComponent* highlight;
	Vec2 targetPos;
public:
	PickerBehaviour(City* c) : c(c) 
	{ 
		LE::Texture hlTex("data/highlight.png");
		highlight = new LE::SpriteComponent(hlTex, 2.0f);
	}
	virtual void Update(float dt);
	virtual void Draw(sf::RenderWindow& window, const sf::Transform& t)
	{
		if (curMode == PickDest)
		{
			sf::Transform trans = t;
			Vec2 isoPos = c->WorldToIso(targetPos);
			trans.translate(sf::Vector2f(isoPos.x-1.0f, isoPos.y-0.5f));
			highlight->Draw(window, trans);

		}
	}
	virtual Component* Clone() const { return nullptr; }

};

class Picker
{
public:
	Picker(City* c)
	{
		GameObject* o = new GameObject();
		o->AddComponent(new PickerBehaviour(c));
		Game::AddToLevel(o);
	}
};
