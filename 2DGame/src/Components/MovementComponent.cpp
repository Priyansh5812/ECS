#include "MovementComponent.h"
#include <raymath.h>
#include <EntityBase.h>
#include <Sprite.h>
MovementComponent::MovementComponent(EntityBase* attachedEntity) : ComponentBase(attachedEntity), speed(200), direction({0.0f,0.0f})
{
	canTick = true;
}

void MovementComponent::Start()
{	
	Vector2 initPos = { 0.0f , SCREEN_HEIGHT / 3 };
	this->attachedEntity->AddPosition(initPos);
}

void MovementComponent::Update(const float& deltaTime)
{
	DirectionConstruction(deltaTime);
	ApplyDirectionalMovement(deltaTime);
}

void MovementComponent::OnDestroy()
{

}


void MovementComponent::DirectionConstruction(const float& deltaTime)
{
	direction = {0.0f,0.0f};
	direction = Vector2Add(direction, { IsKeyDown(KEY_A) ? -1.0f : 0.0f , 0.0f });
	direction = Vector2Add(direction, { IsKeyDown(KEY_D) ? 1.0f : 0.0f , 0.0f });

	//----- Boundary Check -------
	Vector2 currPos = this->attachedEntity->GetPosition2D();
	Sprite* sprite = this->attachedEntity->TryGetComponent<Sprite>();
	if (sprite)
	{
		Vector2 occupiedArea = sprite->GetArea();

		// WIDTH CHECK
		currPos = currPos + direction * speed * deltaTime;
		if (currPos.x + occupiedArea.x >= SCREEN_WIDTH/2 - 50.0f)
		{
			direction = { 0.0f , 0.0f };
			
		}

		if (currPos.x - occupiedArea.x <= -SCREEN_WIDTH/2 + 50.0f)
		{
			direction = { 0.0f , 0.0f };
		}

	}
}

void MovementComponent::ApplyDirectionalMovement(const float& deltaTime)
{	
	this->attachedEntity->AddPosition(direction * speed * deltaTime);
}

