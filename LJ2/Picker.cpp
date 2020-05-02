#include "Picker.h"
#include "roads.h"

void PickerBehaviour::UpdateSelectPed()
{
	Vec2 pos = Game::GetMousePosWorld();
	Vec2 isoPos = c->IsoToWorld(pos);

	c->ClearPedHighlights();
	// find nearest pedestrian
	int nearPed = c->FindNearestPedestrian(isoPos);
	LE::Vec2 nearPedPos = c->GetPed(nearPed)->GetPos();

	c->GetPed(nearPed)->Highlight(true);

	if (Game::MouseClicked(MouseButtons::MB_LEFT))
	{
		selectedPed = nearPed;
		curMode = PickDest;
		c->GetPed(selectedPed)->Stop();
	}
}

void PickerBehaviour::UpdateSelectDest()
{
	Vec2 pos = Game::GetMousePosWorld();
	Vec2 isoPos = c->IsoToWorld(pos);

	targetPos = c->walkways->NearestPointOnWalkways(isoPos);
	if (Game::MouseClicked(MouseButtons::MB_LEFT))
	{
		c->GetPed(selectedPed)->SetTarget( targetPos );
		curMode = PickPed;
	}

}

void PickerBehaviour::Update(float dt)
{
	if (curMode == PickPed)
	{
		UpdateSelectPed();
	}
	else if (curMode == PickDest)
	{
		UpdateSelectDest();
	}

	// camera movenet
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		Game::GetCamera().SetPos(Game::GetCamera().GetPos() + LE::Vec2(-10.0f * dt, 0.0f));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		Game::GetCamera().SetPos(Game::GetCamera().GetPos() + LE::Vec2(10.0f * dt, 0.0f));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		Game::GetCamera().SetPos(Game::GetCamera().GetPos() + LE::Vec2(0.0f, -10.0f * dt));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		Game::GetCamera().SetPos(Game::GetCamera().GetPos() + LE::Vec2(0.0f, 10.0f * dt));
}