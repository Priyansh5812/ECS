#include "ShooterComponent.h"
#include <iostream>
#include <BulletFactory.h>
#include <EntityBase.h>
#include <raylib.h>
ShooterComponent::ShooterComponent(EntityBase* attachedEntity) : ComponentBase(attachedEntity) , shootKeyCode(KEY_SPACE)
{
	std::cout << "ShooterComponent Created" << std::endl;
	canTick = true;
}

// This will execute before the runtime starts
void ShooterComponent::Start() 
{

}

// This will execute every frame
void ShooterComponent::Update(const float& deltaTime)
{
	if (IsKeyPressed(shootKeyCode))
	{
		Fire();
	}
}

// This will execute once the game ends
void ShooterComponent::OnDestroy()
{

}

// Simple firing logic 
void ShooterComponent::Fire()
{
	EntityBase* bullet = BulletFactory::PoolGetBullet();
	bullet->SetPosition(this->attachedEntity->GetPosition2D());
	Vector2 pos = { -22.5f , -45.0f };
	if (!useLeftTurret)
	{
		pos.x *= -1.0f;
	}
	bullet->AddPosition(pos);
	bullet->SetActive(true);
	useLeftTurret = !useLeftTurret;
}
