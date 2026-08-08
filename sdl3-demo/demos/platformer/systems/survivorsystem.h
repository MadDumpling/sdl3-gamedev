#pragma once

#include <systems/system.h>

#include "../components/survivorcomponent.h"
#include <components/physicscomponent.h>

class InteractEvent;

class SurvivorSystem : public System<FrameStage::Gameplay, SurvivorComponent>
{
public:
	SurvivorSystem(Services &services);
	void update(Node &node) override;
	void onEvent(NodeHandle target, const InteractEvent &event);
};
