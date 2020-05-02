#pragma once
#include "Texture.h"
#include <vector>

namespace LE
{
	class Animation
	{
		std::vector<Texture*> anim;
		float frameRate;
		float frameTime;
		bool loop;
	public:
		Animation(std::vector<Texture*> textures, float frameRate, bool loop) :
			frameRate(frameRate), frameTime(1.0f / frameRate), loop(loop), anim(textures) {}
		float GetFrameTime() const { return frameTime; }
		float GetFrameRate() const { return frameRate; }
		unsigned int GetNumFrames() const { return anim.size(); }
		Texture* GetFrame(unsigned int i) const { return anim[i]; }
		bool GetLooping() const { return loop; }
	};
}