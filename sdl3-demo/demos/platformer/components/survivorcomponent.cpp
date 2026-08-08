#include "survivorcomponent.h"

SurvivorComponent::SurvivorComponent(Node &owner) : Component(owner, FrameStage::Gameplay), state(SurvivorState::waiting), followOffset(0.0f)
{
}
