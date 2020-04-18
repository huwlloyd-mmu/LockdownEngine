#pragma once
#include "Component.h"
#include "SpriteComponent.h"
#include "Animation.h"
#include <map>

namespace LE
{
	class AnimatedSpriteComponent : public Component
	{
		SpriteComponent* sc; // we use the existing SpriteComponent for the rendering - this just wraps it and changes the texture
		std::map<std::string, Animation*> modes;
		std::string curMode;
		float curTime;
		float worldSize;
	public:
		AnimatedSpriteComponent(float worldSize) : worldSize(worldSize)
		{
			sc = new SpriteComponent(worldSize);
		}
		void AddMode(const std::string& name, Animation* anim)
		{
			modes[name] = anim;
		}
		void StartMode(const std::string& mode, bool sync=false)
		{
			if (!sync)
				curTime = 0.0f;
			curMode = mode;
		}
		virtual void Update(float dt)
		{
			curTime += dt;
			// check for looping
			Animation* curAnim = modes[curMode];
			float duration = curAnim->GetFrameTime() * curAnim->GetNumFrames();
			if (curAnim->GetLooping())
				curTime = fmod(curTime, duration);
			else
				curTime = fmax(curTime, duration);
			int iFrame = curTime * curAnim->GetFrameRate();
			Texture* curTexture = curAnim->GetFrame(iFrame);
			sc->SetTexture(*curTexture);
		}
		virtual void Draw(sf::RenderWindow& window, const sf::Transform& transform)
		{
			sc->Draw(window, transform);
		}
		virtual AnimatedSpriteComponent* Clone() const
		{
			AnimatedSpriteComponent* newSc = new AnimatedSpriteComponent(worldSize);
			newSc->sc->spriteFlags = sc->spriteFlags;
			for (auto it = modes.begin(); it != modes.end(); ++it)
			{
				newSc->AddMode(it->first, it->second);
			}
			return newSc;
		}
		void SetClip() { sc->SetClip(); }
		void SetNoClip() { sc->SetNoClip(); }
		void SetSort() { sc->SetSort(); }
		void SetNoSort() { sc->SetNoSort(); }

	};
}