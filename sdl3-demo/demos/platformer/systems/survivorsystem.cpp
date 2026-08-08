#include "survivorsystem.h"

#include <framecontext.h>
#include <glm/glm.hpp>
#include <inputstate.h>
#include <limits>
#include <functional>
#include <messaging/events.h>
#include <world.h>
#include "../components/hungercomponent.h"

SurvivorSystem::SurvivorSystem(Services &services) : System(services)
{
	services.eventQueue().dispatcher.registerHandler<InteractEvent>(this);
}

void SurvivorSystem::update(Node &node)
{
	auto [sc] = getRequiredComponents(node);
	if (sc->getState() != SurvivorState::following)
	{
		return;
	}

	NodeHandle playerHandle = services.inputState().getFocusTarget();
	if (!playerHandle.isValid())
	{
		return;
	}

	Node &player = services.world().getNode(playerHandle);
	glm::vec2 targetPosition = player.getPosition() + sc->getFollowOffset();
	glm::vec2 currentPosition = node.getPosition();
	glm::vec2 direction = targetPosition - currentPosition;
	float distance = glm::length(direction);
	if (distance > 1.0f)
	{
		direction = glm::normalize(direction);
		node.setPosition(currentPosition + direction * std::min(distance, FrameContext::dt() * 160.0f));
	}
}

void SurvivorSystem::onEvent(NodeHandle target, const InteractEvent &event)
{
	NodeHandle playerHandle = services.inputState().getFocusTarget();
	if (!playerHandle.isValid())
	{
		return;
	}

	Node &player = services.world().getNode(playerHandle);
	auto *hc = player.getComponent<HungerComponent>();
	if (!hc)
	{
		return;
	}

	const float rescueRange = 48.0f;
	NodeHandle closestSurvivor;
	float closestDistance = std::numeric_limits<float>::max();

	std::function<void(NodeHandle)> findClosest = [&](NodeHandle handle)
	{
		Node &node = services.world().getNode(handle);
		auto *sc = node.getComponent<SurvivorComponent>();
		if (sc && sc->getState() == SurvivorState::waiting)
		{
			float dist = glm::distance(node.getPosition(), player.getPosition());
			if (dist < rescueRange && dist < closestDistance)
			{
				closestDistance = dist;
				closestSurvivor = handle;
			}
		}
		for (NodeHandle child : node.getChildren())
		{
			findClosest(child);
		}
	};

	NodeHandle rootHandle = playerHandle;
	while (rootHandle.isValid())
	{
		NodeHandle parent = services.world().getNode(rootHandle).getParent();
		if (!parent.isValid())
		{
			break;
		}
		rootHandle = parent;
	}

	findClosest(rootHandle);

	if (!closestSurvivor.isValid())
	{
		return;
	}

	Node &survivor = services.world().getNode(closestSurvivor);
	auto *sc = survivor.getComponent<SurvivorComponent>();
	if (!sc)
	{
		return;
	}

	sc->setState(SurvivorState::following);
	int index = hc->survivorsRescued;
	float angle = index * 0.8f;
	sc->setFollowOffset(glm::vec2(std::cos(angle), std::sin(angle)) * 40.0f);
	hc->survivorsRescued += 1;
}
