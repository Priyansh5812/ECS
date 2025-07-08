#include "CustomComponent.h"
#include <iostream>
#include <raylib.h>
#include <memory>

CustomComponent::CustomComponent(Entity* attachedEntity) : ComponentBase(attachedEntity)
{
	std::cout << "Custom Component Created" << std::endl;
}

void CustomComponent::Start()
{
	std::cout << "Hello from Custom Component" << std::endl;
}

void CustomComponent::Update(const float& deltaTime)
{
	std::cout << "Updating Custom_delta: " << deltaTime << std::endl;
}


void CustomComponent::OnDestroy()
{
	std::cout << "Destroyed " << std::endl;
}

