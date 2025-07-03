#include "ComponentBase.h"
#include <Entity.h>
#include <iostream>
ComponentBase::ComponentBase(Entity* attachedEntity)
{
	this->attachedEntity = attachedEntity;
	std::cout << "Entity Attached" << std::endl;
}


