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

void ShooterComponent::Start()
{

}

void ShooterComponent::Update(const float& deltaTime)
{
	if (IsKeyPressed(shootKeyCode))
	{
		EntityBase* bullet = BulletFactory::PoolGetBullet();
		bullet->SetPosition(this->attachedEntity->GetPosition2D());
		Vector2 pos = { -22.5f , -45.0f };
		bullet->AddPosition(pos);
		bullet->SetActive(true);
	}
}

void ShooterComponent::OnDestroy()
{
}
