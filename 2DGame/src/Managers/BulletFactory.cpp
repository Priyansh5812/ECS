#include "BulletFactory.h"
#include <EntityBase.h>
#include <ECSManager.h>
#include <BulletComponent.h>

std::queue<EntityBase*>* BulletFactory::bulletPool = nullptr;
bool BulletFactory::isInitialized = false;	

void BulletFactory::EnsureCreation()
{
	if (BulletFactory::isInitialized)
		return;

	bulletPool = new std::queue<EntityBase*>();
}

EntityBase* BulletFactory::PoolGetBullet()
{	
	EntityBase* ent = nullptr;

	if (bulletPool->size())
	{	
		ent = bulletPool->front();
		bulletPool->pop();
	}
	else
	{
		ent = BulletFactory::ConstructBullet();
	}
	
	return ent;
}

void BulletFactory::PoolSetBullet(EntityBase* bullet)
{	
	bullet->SetActive(false);
	bulletPool->push(bullet);
}

EntityBase* BulletFactory::ConstructBullet()
{		
	Entity* en = ECSManager::CreateEntity<Entity>();
	en->SetActive(false);
	en->TryAddComponent<BulletComponent>();

	return en;
}


void BulletFactory::Cleanup()
{	
	//Clearing the pool
	while (bulletPool->size())
	{
		bulletPool->pop();
	}
}