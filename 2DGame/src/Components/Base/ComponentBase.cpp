#include "ComponentBase.h"
#include <EntityBase.h>
#include <iostream>
ComponentBase::ComponentBase(EntityBase* attachedEntity)
{
	this->attachedEntity = attachedEntity;	
}

//void ComponentBase::Update(const float& deltaTime)
//{ 
//	//noop
//}

const EntityBase* ComponentBase::GetAttachedEntity() const
{
	return this->attachedEntity;
}

const bool& ComponentBase::GetCanTick() const
{
	return this->canTick;
}

