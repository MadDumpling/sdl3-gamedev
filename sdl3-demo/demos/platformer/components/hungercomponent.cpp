#include "hungercomponent.h"

HungerComponent::HungerComponent(Node &owner, float maxFood) : Component(owner, FrameStage::Gameplay), food(maxFood), maxFood(maxFood), survivorsRescued(0), starvationStage(0), starvationTimer(0.0f)
{
}
