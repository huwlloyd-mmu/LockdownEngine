#pragma once
#include "level.h"
#include <vector>
#include "WindowInfo.h"
#include "AssetManager.h"
#include "Timer.h"
#include "Types.h"
#include "DebugPrint.h"

namespace LE
{
	// this is the main class with everything in. I think it should probably be all static. You can't really have
	// more than one game.
	class Game
	{
		// levels
		static std::vector<Level*> levels;
		static int curLevel;
		// window
		static WindowInfo windowInfo;
		// asset manager
		static AssetManager assets;
		// timer
		static Timer timer;
		// mouse stuff
		static int mbBuffer;
		static bool mouseButtons[2][MouseButtons::MB_COUNT];
		static Vec2 mousePos;
		// debug text
		static DebugPrint dbg;
		// main loop functions
		static void ClearInputs(void);
		static void UpdateInputs(void);
		static void Update(float dt);
		static void Draw(sf::RenderWindow& window);
	public:
		static void SetWindow(int width, int height, const std::string& title, bool fullscreen);
		static void Run(void);
		static AssetManager& Assets() { return assets; }
		static void AddLevel(Level* level) { levels.push_back(level); }
		static GameObject* SpawnCopy(GameObject* o) 
		{
			GameObject* newObj = o->Clone();
			levels[curLevel]->AddObject(newObj); 
			return newObj;
		}
		static void DestroyObject(GameObject* obj)
		{
			levels[curLevel]->DeleteObject(obj);
			delete obj;
		}
		static bool MouseDown(const MouseButtons& button) { return mouseButtons[mbBuffer][button]; }
		static bool MouseClicked(const MouseButtons& button) { return !mouseButtons[mbBuffer][button] && mouseButtons[mbBuffer^1][button]; }
		static const Vec2& GetMousePos() { return mousePos; }
		static void DebugOut(const std::string& line) { Game::dbg.AddLine(line); }
		static GameObject* FindObject(const std::string& name);
	};
}
