#pragma once
#include "Component.h"
#include "TextComponent.h"
#include "City.h"

using namespace LE;

class DBGPosBehaviour : public Component
{
	TextComponent* text = nullptr;
	City* c;
public:
	DBGPosBehaviour(City *c) : c(c) { }
	virtual void Update(float dt)
	{
		if ( text == nullptr )
			text = parent->GetComponent<TextComponent>();
		if (text != nullptr)
		{
			Vec2 pos = Game::GetMousePosWorld();
			Vec2 isoPos = c->IsoToWorld(pos);
			parent->SetPosition(Game::GetMousePos());

			std::string message = "" + std::to_string(isoPos.x) + " " + std::to_string(isoPos.y);
			text->SetText(message);
		}
	}
	virtual Component* Clone() const { return new DBGPosBehaviour(c); }
	
};

class DebugPosition
{
public:
	DebugPosition(City *c) 
	{
		GameObject* o = new GameObject();
		TextComponent* t = new TextComponent("data/roboto-regular.ttf", 30);
		t->SetColor(sf::Color(255, 0, 0, 255));
		o->AddComponent(t);
		o->AddComponent(new DBGPosBehaviour(c));
		o->SetUI(true);
		Game::AddToLevel(o);
	}
};