#include "weaponsystem.h"

#include <glm/glm.hpp>

#include "../components/weaponcomponent.h"
#include "../components/projectilecomponent.h"

#include <resources.h>
#include <framecontext.h>
#include <components/animationcomponent.h>
#include <components/collisioncomponent.h>
#include <components/spritecomponent.h>
#include <world.h>
#include <messaging/eventqueue.h>
#include <messaging/events.h>
#include <componentsystems.h>
#include <logger.h>

WeaponSystem::WeaponSystem(Services &services) : System(services)
{
	fireDirection = { 1, 0 };
	services.eventQueue().dispatcher.registerHandler<ShootBeginEvent>(this);
	services.eventQueue().dispatcher.registerHandler<ShootEndEvent>(this);
	services.eventQueue().dispatcher.registerHandler<DirectionChangedEvent>(this);
	services.eventQueue().dispatcher.registerHandler<AimChangedEvent>(this);
	//services.eventQueue().dispatcher.registerHandler<TimerTimeoutEvent>(this);
}

void WeaponSystem::update(Node &node)
{
	auto [wc, pc] = getRequiredComponents(node);

	bool canFire = wc->getCooldownTimer().step(FrameContext::dt());
	if (wc->isShooting() && canFire)
	{
		// restart cooldown timer
		wc->getCooldownTimer().reset();
		canFire = false;

		World &world = services.world();
		NodeHandle handle = world.createNode();
		Node &bullet = world.getNode(handle);
		bullet.setTag(4);

		auto &res = Resources::get();

		auto &physCmp = services.compSys().addComponent<PhysicsComponent>(bullet);
		const glm::vec2 bulletVelocity = pc->getVelocity() + fireDirection * 600.0f;
		physCmp.setVelocity(bulletVelocity);
		physCmp.setMaxSpeed(glm::vec2(1200.0f, 1200.0f));
		//physCmp.setDynamic(true);
		physCmp.setGravityFactor(0);
		physCmp.setDamping(0);

		auto &animCmp = services.compSys().addComponent<AnimationComponent>(bullet, res.bulletAnims);
		animCmp.setAnimation(res.ANIM_BULLET_MOVING);
		auto &rndCmp = services.compSys().addComponent<SpriteComponent>(
			bullet, res.texBullet, static_cast<float>(res.texBullet->h),
			static_cast<float>(res.texBullet->h));
		rndCmp.setFlipMode(fireDirection.x < 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
		rndCmp.setRotation(static_cast<float>(SDL_rand(360)));

		auto &collCmp = services.compSys().addComponent<CollisionComponent>(bullet);
		collCmp.setCollider(SDL_FRect{
			.x = 0, .y = 0,
			.w = 4, .h = 4
			});
		services.compSys().addComponent<ProjectileComponent>(bullet);

		// adjust bullet start position
		glm::vec2 offset = fireDirection * 18.0f;
		bullet.setPosition(node.getPosition() + offset);

		node.addChild(bullet);
	}
}

void WeaponSystem::onEvent(NodeHandle target, const ShootBeginEvent &event)
{
	Node &node = services.world().getNode(target);
	if (node.isLinkedWith(this))
	{
		auto [wc, pc] = getRequiredComponents(node);
		if (!wc->isShooting())
		{
			wc->setIsShooting(true);
		}
	}
}

void WeaponSystem::onEvent(NodeHandle target, const ShootEndEvent &event)
{
	Node &node = services.world().getNode(target);
	if (node.isLinkedWith(this))
	{
		auto [wc, pc] = getRequiredComponents(node);
		if (wc->isShooting())
		{
			wc->setIsShooting(false);
		}
	}
}

void WeaponSystem::onEvent(NodeHandle target, const DirectionChangedEvent &event)
{
	// we only care if we're turning (-1, 1)
	if (event.getDirection().x != 0)
	{
		fireDirection = event.getDirection();
	}
}

void WeaponSystem::onEvent(NodeHandle target, const AimChangedEvent &event)
{
	if (glm::length(event.getDirection()) > 0.001f)
	{
		fireDirection = glm::normalize(event.getDirection());
	}
}
