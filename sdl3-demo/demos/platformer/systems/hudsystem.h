#pragma once

#include <systems/system.h>

class HudSystem : public System<FrameStage::Render>
{
public:
	HudSystem(Services &services);
	void update(Node &node) override;
};
