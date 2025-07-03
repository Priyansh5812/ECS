#pragma once
#include<ComponentBase.h>

class Entity;

class CustomComponent : public ComponentBase
{
public:
	CustomComponent(Entity* attachedEntity);
	virtual void Start() override; // Initialization 
	virtual void Update(float deltaTime) override; // OnTick
	virtual void OnDestroy() override;// Deinitialization
};

