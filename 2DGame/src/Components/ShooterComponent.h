#pragma once
#include <ComponentBase.h>

class ShooterComponent : public ComponentBase
{
public:
	ShooterComponent(EntityBase* attachedEntity);
	virtual void Start() override; // Initialization 
	virtual void Update(const float& deltaTime) override; // OnTick
	virtual void OnDestroy() override;// Deinitialization
private:
	int shootKeyCode;
	
};

