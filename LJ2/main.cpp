#include "ShadowGame.h"
#include <memory>
void main()
{
	std::unique_ptr<ShadowGame> game = std::make_unique<ShadowGame>();
	game->Run();
}