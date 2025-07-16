#pragma once
#include <ComponentBase.h>
#include <raylib.h>
class MovementComponent : public ComponentBase
{
public:
	MovementComponent(EntityBase* attachedEntity);
	virtual void Start() override; // Initialization 
	virtual void Update(const float& deltaTime) override; // OnTick
	virtual void OnDestroy() override;	// Deinitialization
	void DirectionConstruction(const float& deltaTime);
	void ApplyDirectionalMovement(const float& deltaTime);

private:
	float speed;
	Vector2 direction;

};


