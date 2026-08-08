#include "hungersystem.h"

#include <algorithm>
#include <framecontext.h>
#include <glm/glm.hpp>

HungerSystem::HungerSystem(Services &services) : System(services)
{
}

void HungerSystem::update(Node &node)
{
	auto [hc, pc] = getRequiredComponents(node);

	const float baseDrain = 4.0f;
	const float survivorDrain = 0.8f;
	const float dt = FrameContext::dt();
	const float totalDrain = baseDrain + survivorDrain * static_cast<float>(hc->survivorsRescued);

	hc->food = std::clamp(hc->food - totalDrain * dt, 0.0f, hc->maxFood);

	if (hc->food > 0.0f)
	{
		hc->starvationStage = 0;
		hc->starvationTimer = 0.0f;
	}
	else
	{
		hc->starvationTimer += dt;
		if (hc->starvationTimer > 12.0f)
		{
			hc->starvationStage = 3;
		}
		else if (hc->starvationTimer > 7.0f)
		{
			hc->starvationStage = 2;
		}
		else
		{
			hc->starvationStage = 1;
		}

		if (hc->starvationStage == 3 && hc->survivorsRescued > 0 && hc->starvationTimer > 14.0f)
		{
			hc->survivorsRescued -= 1;
			hc->starvationTimer = 10.0f;
		}
	}

	const glm::vec2 baseMaxSpeed(100.0f, 300.0f);
	float slowFactor = 1.0f - 0.15f * static_cast<float>(hc->starvationStage);
	glm::vec2 slowedSpeed = baseMaxSpeed * slowFactor;
	slowedSpeed.x = std::max(20.0f, slowedSpeed.x);
	slowedSpeed.y = std::max(20.0f, slowedSpeed.y);
	pc->setMaxSpeed(slowedSpeed);
}
