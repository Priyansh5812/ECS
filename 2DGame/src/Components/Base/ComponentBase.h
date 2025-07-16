#pragma once
#include <Constants.h>
#include <memory>
class EntityBase;


class ComponentBase		
{	
public:	
	ComponentBase(EntityBase* entity);	

public:
	virtual void Start() = 0; // Initialization 
	virtual void Update(const float& deltaTime) = 0; // OnTick
	virtual void OnDestroy() = 0;// Deinitialization 

public:
	const EntityBase* GetAttachedEntity() const;
	const bool& GetCanTick() const;

protected:
	EntityBase* attachedEntity;	
	bool canTick = false;
	
};




