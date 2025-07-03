#pragma once
#include <Constants.h>
class Entity;


class ComponentBase		
{	
public:	
	ComponentBase(Entity* entity);	
	virtual void Start() = 0; // Initialization 
	virtual void Update(float deltaTime) = 0; // OnTick
	virtual void OnDestroy() = 0;// Deinitialization 

		
protected:
	Entity* attachedEntity = nullptr;	
	bool canTick = false;
	
};




