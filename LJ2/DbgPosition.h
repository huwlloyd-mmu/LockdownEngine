#pragma once
#include "Component.h"
#include "TextComponent.h"
#include "City.h"
#include "DebugLines.h"
#include "Walkways.h"
#include <iomanip>
#include <iostream>
#include <sstream>
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
			std::stringstream ss;
			ss << std::setprecision(4) << isoPos.x << " " << std::setprecision(4) << isoPos.y <<
				" " << std::setprecision(4) << pos.x << " " << std::setprecision(4) << pos.y;
			text->SetText(ss.str());
			if (Game::MouseClicked(MouseButtons::MB_LEFT))
			{
				Game::GetCamera().BlendTo(pos, 0.01f);
			}
		}
	}
	virtual void Draw(sf::RenderWindow& window, const sf::Transform& t)
	{
		// some lines
		std::vector<Vec2> dbglines;
		for (auto e : c->walkways->e)
		{
			dbglines.push_back(c->WorldToIso(c->walkways->v[e.v0].pos));
			dbglines.push_back(c->WorldToIso(c->walkways->v[e.v1].pos));
		}
		DebugLines::Draw(dbglines, sf::Color(0, 255, 0, 255));
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