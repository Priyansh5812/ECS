#pragma once
#include <Constants.h>
#include <memory>
class Entity;


class ComponentBase		
{	
public:	
	ComponentBase(Entity* entity);	
	virtual void Start() = 0; // Initialization 
	virtual void Update(const float& deltaTime); // OnTick
	virtual void OnDestroy() = 0;// Deinitialization 
	const Entity* GetAttachedEntity() const;
	const bool& GetCanTick() const;
protected:
	Entity* attachedEntity;	
	bool canTick = false;
	
};




