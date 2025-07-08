#include "ComponentBase.h"
#include <Entity.h>
#include <iostream>
ComponentBase::ComponentBase(Entity* attachedEntity)
{
	this->attachedEntity = attachedEntity;	
	std::cout << "Entity Attached" << std::endl;	
}

void ComponentBase::Update(const float& deltaTime)
{ 
	//noop
}

const Entity* ComponentBase::GetAttachedEntity() const
{
	return this->attachedEntity;
}

const bool& ComponentBase::GetCanTick() const
{
	return this->canTick;
}

