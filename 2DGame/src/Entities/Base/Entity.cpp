#include "Entity.h"
#include <typeinfo>

Entity::Entity()
{	
	RegisterEntity();
	this->InitializeTransform();
}


void Entity::InitializeTransform()
{	
	Transform = MatrixIdentity(); // Reset all the values
}

void Entity::RegisterEntity()
{	
	this-> uid = ECSManager::GetConstInstance()->RegisterEntity(this);
}

const Vector3& Entity::GetPosition()
{
	this->m_position = { Transform.m12,Transform.m13,Transform.m14 };
	return this->m_position;
}

void Entity::AddPosition(const Vector3& position)
{	
	Matrix tm = MatrixTranslate(position.x , position.y, position.z);
	Transform = MatrixAdd(Transform, tm);
}

void Entity::Cleanup()
{

}

const int& Entity::GetUID()const
{
	return uid;
}



ComponentBase* Entity::TryGetComponent(std::type_info& type)
{	
	return nullptr;
}

void Entity::TryRemoveComponent(std::type_info& type)
{

}




