#pragma once
#include "Component.h"
#include "SpriteComponent.h"

using namespace LE;

class Gem : public Component
{
	Texture base;
	Texture hl;
	bool bHighlight = false;
	float tx, ty; // target position
	bool bMoving = false;
	const float speed = 400.0f;
	float signof(float x) { if (x < 0.0f) return -1.0f; else return 1.0f; }
	float throb = 0.0f;
public:
	Gem() {}

	void SetTextures(const Texture& nbase, const Texture& nhl)
	{
		base = nbase;
		hl = nhl;
		SpriteComponent* s = parent->GetComponent<SpriteComponent>();
		if (bHighlight)
			s->SetTexture(hl);
		else
			s->SetTexture(base);
	}

	void SetTarget(float x, float y)
	{
		tx = x; ty = y;
	}
	virtual void Init() {};
	virtual void Update(float dt) 
	{
		// pulse if highlighted
		if (bHighlight)
		{
			float scale = 1.0 + sin(throb*0.8f) * 0.1f;
			parent->SetScale(scale, scale);
		}
		else
			parent->SetScale(1.0f, 1.0f);
		throb += dt*20.0f;
		bMoving = false;
		float eps = 1e-3f;
		Vec2 pos = parent->GetPosition();
		if (fabs(pos.x - tx) > eps)
		{
			bMoving = true;
			float delta = tx - pos.x;
			if (fabs(delta) > speed * dt)
			{
				delta = speed * dt * signof(delta);
			}
			pos.x += delta;
		}
		else
		{
			pos.x = tx;
		}
		if (fabs(pos.y - ty) > eps)
		{
			bMoving = true;
			float delta = ty - pos.y;
			if (fabs(delta) > speed * dt)
			{
				delta = speed * dt * signof(delta);
			}
			pos.y += delta;
		}
		else
		{
			pos.y = ty;
		}
		parent->SetPosition(pos.x, pos.y);
	};

	bool Moving() const { return bMoving; }
	void Highlight(bool highlight)
	{
		bHighlight = highlight;
	}

	bool GetHighlight() const { return bHighlight; }
	virtual void Draw() {};
	virtual Gem* Clone() const 
	{ 
		Gem *gem = new Gem(); 
		gem->base = base;
		gem->hl = hl;
		return gem;
	}
};