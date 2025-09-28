#pragma once
#include <ComponentBase.h>

class Sprite;



class BulletComponent : public ComponentBase
{
public:
	BulletComponent(EntityBase* attachedEntity);
	virtual void Start() override; // Initialization 
	virtual void Update(const float& deltaTime) override; // OnTick
	virtual void OnDestroy() override;// Deinitialization
private:
	void BulletMove(const float& deltaTime);
	void MonitorTimeout(const float& deltaTime);
	void Reset();
private:
	Sprite* sprite;
	float bulletTimer = 3.0f;
};

