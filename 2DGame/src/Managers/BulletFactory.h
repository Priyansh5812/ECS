#pragma once
#include <queue>
#include <memory>
class EntityBase;

class BulletFactory
{
public:
	static void EnsureCreation();
	static void Cleanup();

public:
	static EntityBase* PoolGetBullet();
	static void PoolSetBullet(EntityBase* bullet);

private:
	static bool isInitialized;
	BulletFactory();
	BulletFactory(const BulletFactory& instance) = delete;

private:
	static std::queue<EntityBase*>* bulletPool;

private:
	static EntityBase* ConstructBullet();

};

