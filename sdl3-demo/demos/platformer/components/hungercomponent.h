#pragma once

#include <components/component.h>

class HungerComponent : public Component
{
public:
	float food;
	float maxFood;
	int survivorsRescued;
	int starvationStage;
	float starvationTimer;

	HungerComponent(Node &owner, float maxFood = 100.0f);
};
