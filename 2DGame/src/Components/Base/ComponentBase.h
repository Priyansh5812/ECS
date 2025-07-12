#pragma once
#include <Constants.h>
#include <memory>
class EntityBase;


class ComponentBase		
{	
public:	
	ComponentBase(EntityBase* entity);	
	virtual void Start() = 0; // Initialization 
	virtual void Update(const float& deltaTime); // OnTick
	virtual void OnDestroy() = 0;// Deinitialization 
	const EntityBase* GetAttachedEntity() const;
	const bool& GetCanTick() const;
protected:
	EntityBase* attachedEntity;	
	bool canTick = false;
	
};




