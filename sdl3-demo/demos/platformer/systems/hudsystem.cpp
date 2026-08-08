#include "hudsystem.h"

#include <format>
#include <SDL3/SDL.h>
#include <sdlstate.h>
#include <inputstate.h>
#include "../components/healthcomponent.h"
#include "../components/hungercomponent.h"
#include <world.h>
#include <messaging/events.h>

HudSystem::HudSystem(Services &services) : System(services)
{
}

void HudSystem::update(Node &node)
{
	if (node.getParent().isValid())
	{
		return;
	}

	NodeHandle playerHandle = services.inputState().getFocusTarget();
	if (!playerHandle.isValid())
	{
		return;
	}

	Node &player = services.world().getNode(playerHandle);
	auto *hc = player.getComponent<HungerComponent>();
	auto *health = player.getComponent<HealthComponent>();
	if (!hc || !health)
	{
		return;
	}

	SDL_Renderer *renderer = SDLState::global().renderer;
	if (!renderer)
	{
		return;
	}

	std::string hudText = std::format("Health: {}  Food: {}/{}  Rescued: {}",
		health->hp,
		static_cast<int>(hc->food),
		static_cast<int>(hc->maxFood),
		hc->survivorsRescued);

	SDL_RenderDebugText(renderer, 10, 10, hudText.c_str());
}
