#pragma once
#include<ComponentBase.h>
#include <Entity.h>

class CustomComponent : public ComponentBase 
{
public:
	CustomComponent(Entity* attachedEntity);
	virtual void Start() override; // Initialization 
	virtual void Update(const float& deltaTime) override; // OnTick
	virtual void OnDestroy() override;// Deinitialization
};

