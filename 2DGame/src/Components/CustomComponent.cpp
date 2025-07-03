#include "CustomComponent.h"
#include <Entity.h>
#include <iostream>

CustomComponent::CustomComponent(Entity* attachedEntity) : ComponentBase(attachedEntity)
{
	std::cout << "Custom Component Created" << std::endl;
}


void CustomComponent::Start()
{
	std::cout << "Hello from Custom Component" << std::endl;
}

void CustomComponent::Update(float deltaTime)
{
}


void CustomComponent::OnDestroy()
{
}
