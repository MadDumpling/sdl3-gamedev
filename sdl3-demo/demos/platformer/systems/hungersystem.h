#pragma once

#include <systems/system.h>

#include "../components/hungercomponent.h"
#include <components/physicscomponent.h>

class HungerSystem : public System<FrameStage::Gameplay, HungerComponent, PhysicsComponent>
{
public:
	HungerSystem(Services &services);
	void update(Node &node) override;
};
