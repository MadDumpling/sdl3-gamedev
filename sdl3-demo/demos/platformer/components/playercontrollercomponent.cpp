#include "playercontrollercomponent.h"

#include <framecontext.h>
#include <messaging/messaging.h>
#include <node.h>
#include <logger.h>

PlayerControllerComponent::PlayerControllerComponent(Node &owner)
	: Component(owner, FrameStage::Gameplay), slideTimer(0.16f), parryTimer(0.2f), dashTimer(0.18f), dashCooldownTimer(0.8f)
{
	direction = 0;
	velocity = glm::vec2(0);
	grounded = false;
	shooting = false;
	parrying = false;
	dashing = false;
	dashReady = true;
	dashDirection = glm::vec2(1, 0);
	currentState = PState::idle;
	idleAnimationIndex = 0;
	idleTexture = nullptr;
	runAnimationIndex = 0;
	runTexture = nullptr;
	jumpAnimationIndex = 0;
	jumpTexture = nullptr;
	slideAnimationIndex = 0;
	slideTexture = nullptr;
	shootAnimationIndex = 0;
	shootTexture = nullptr;
}

void PlayerControllerComponent::onCommand(const UpdateVelocityCommand &msg)
{
	this->velocity = msg.getVelocity();
}

void PlayerControllerComponent::onCommand(const UpdateDirectionCommand &msg)
{
	this->direction = msg.getDirection();
}

void PlayerControllerComponent::startDash(const glm::vec2 &direction)
{
	if (!dashReady)
	{
		return;
	}
	dashReady = false;
	dashing = true;
	dashDirection = glm::length(direction) > 0.001f ? glm::normalize(direction) : dashDirection;
	dashTimer.reset();
	dashCooldownTimer.reset();
}

void PlayerControllerComponent::startParry()
{
	if (parrying)
	{
		return;
	}
	parrying = true;
	parryTimer.reset();
}

