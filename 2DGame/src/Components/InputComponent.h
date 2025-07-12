#pragma once
#include<ComponentBase.h>
class InputComponent : public ComponentBase
{
public:
	InputComponent(EntityBase* attachedEntity);
	virtual void Start() override; // Initialization 
	virtual void Update(const float& deltaTime) override; // OnTick
	virtual void OnDestroy() override;// Deinitialization

};

