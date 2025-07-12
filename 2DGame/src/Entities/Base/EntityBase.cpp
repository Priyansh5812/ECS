#include "EntityBase.h"
#include <typeinfo>

EntityBase::EntityBase()
{	
	RegisterEntity();
	this->InitializeTransform();
}


void EntityBase::InitializeTransform()
{
	Transform = MatrixIdentity(); // Reset all the values
}

void EntityBase::RegisterEntity()
{
	this->globalEntityIndex = ECSManager::GetConstInstance()->RegisterEntity(this);
}

const Vector3& EntityBase::GetPosition()
{
	this->m_position = { Transform.m12,Transform.m13,Transform.m14 };
	return this->m_position;
}

void EntityBase::AddPosition(const Vector3& position)
{
	Matrix tm = MatrixTranslate(position.x, position.y, position.z);
	Transform = MatrixAdd(Transform, tm);
}

EntityBase::~EntityBase()
{	
	std::cout << "Base entity destroyed" << std::endl;
}










