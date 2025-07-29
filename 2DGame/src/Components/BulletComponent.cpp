#include "BulletComponent.h"
#include <ECSManager.h>
#include <EntityBase.h>
#include <Sprite.h>
#include <BulletFactory.h>
BulletComponent::BulletComponent(EntityBase* attachedEntity) : ComponentBase(attachedEntity)
{
	canTick = true;
}

void BulletComponent::Start()
{
	sprite = this->attachedEntity->TryAddComponent<Sprite>("./Images/Laser.png", Vector2{25.0f, 40.0f});
}

void BulletComponent::Update(const float& deltaTime)
{
	BulletMove(deltaTime);
	MonitorTimeout(deltaTime);
}

void BulletComponent::OnDestroy()
{

}

void BulletComponent::BulletMove(const float& deltaTime)
{
	Vector2 add = { 0.0f , -5.0f * 50.0f * deltaTime };
	this->attachedEntity->AddPosition(add);
}

void BulletComponent::MonitorTimeout(const float& deltaTime)
{
	if (bulletTimer <= 0.0f)
	{	
		this->Reset();
		return;
	}

	// Otherwise...
	bulletTimer -= deltaTime;
}

void BulletComponent::Reset()
{
	BulletFactory::PoolSetBullet(this->attachedEntity);
	bulletTimer = 3.0f;
}