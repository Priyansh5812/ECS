#include "InputComponent.h"
#include <EntityBase.h>
#include <iostream>
#include <raylib.h>
InputComponent::InputComponent(EntityBase* attachedEntity) : ComponentBase(attachedEntity)
{
	canTick = true;
}

void InputComponent::Start()
{
	std::cout << "Started Input Comp" << std::endl;
}

void InputComponent::Update(const float& deltaTime)
{
	if (IsKeyDown(KEY_W)) {
		this->attachedEntity->AddPosition(Vector3UnitY * (deltaTime * -200.0f));
	}
	if (IsKeyDown(KEY_A)) {
		this->attachedEntity->AddPosition(Vector3UnitX * (deltaTime * -200.0f));
	}
	if (IsKeyDown(KEY_S)) {
		this->attachedEntity->AddPosition(Vector3UnitY * (deltaTime * 200.0f));
	}
	if (IsKeyDown(KEY_D)) {
		this->attachedEntity->AddPosition(Vector3UnitX * (deltaTime * 200.0f));
	}
}

void InputComponent::OnDestroy()
{
	std::cout << "Destroyed Input Comp" << std::endl;
}
