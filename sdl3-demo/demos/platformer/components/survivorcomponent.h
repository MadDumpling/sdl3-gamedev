#pragma once

#include <components/component.h>
#include <glm/glm.hpp>

enum class SurvivorState
{
	waiting,
	rescued,
	following
};

class SurvivorComponent : public Component
{
	SurvivorState state;
	glm::vec2 followOffset;

public:
	SurvivorComponent(Node &owner);

	SurvivorState getState() const { return state; }
	void setState(SurvivorState newState) { state = newState; }

	const glm::vec2 &getFollowOffset() const { return followOffset; }
	void setFollowOffset(const glm::vec2 &offset) { followOffset = offset; }
};
