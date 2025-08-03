# Space Invaders via Custom Entity Component System (ECS)
This readme will guide you through the architecture I followed in order to make the ECS.


## The ECS Core
The Entity Component System gets managed by a single class ```ECSManager``` and is divided for Entities and Components separately.

### Data Storage Paradigm

1. The Entity Directory :  <br></br>
Denotion : ```ECSManager::entityDir``` <br></br>
![EntityDir](https://github.com/Priyansh5812/Space-Invaders/blob/main/Readme%20Imgs/EntityDir.png)
2. The Global Component List : <br></br>
Denotion: ```ECSManager::globalComponentList```<br></br>
![Global Component List](https://github.com/Priyansh5812/Space-Invaders/blob/main/Readme%20Imgs/globalComponentList.png)
4. Runtime Calls : 
![Runtime Calls](https://github.com/Priyansh5812/Space-Invaders/blob/main/Readme%20Imgs/Runtime%20Calls.png)

### Entity Handling

```cpp
template<typename T>
inline T*& ECSManager::CreateEntity()
{	
	if (std::is_abstract<T>()) // class can't be abstract
	{
		throw std::runtime_error("Entity can't be an abstract class"); 
	}

	if (!std::is_base_of<EntityBase, T>()) // class must have "EntityBase" as its Parent class
	{
		std::string str = typeid(T).name();
		str.append(" is not inheriting EntityBase class");
		throw std::runtime_error(str);
	}
	
	T* ptr = new T(); 
	ptr->OnCreated(); // OnCreated Callback
	return ptr;
}
```
Here the entity creation is done by ```ECSManager::CreateEntity()``` rather than ```new``` keyword.
Upon successfull creation of an Entity, its ```Entity::OnCreated()``` gets invoked.

##


Similarly the Destruction of the entity gets also handled by the another static method ```ECSManager::DestroyEntity(Entity entity)```
```cpp
template<typename T1>
inline void ECSManager::DestroyEntity(T1* entity)
{
	
	if (entity == nullptr || dynamic_cast<Entity*>(entity) == nullptr)
	{	
		std::cout << "Entity not destroyed, because it does not inherit from class 'Entity' or 'EntityBase'" << std::endl;
		return;
	}

	// Unregistering all the components from the system
	auto& dir = ECSManager::GetInstance()->entityDir;

	if (dir.find(entity) != dir.end())
	{
		std::shared_ptr<EntityData> data = dir[entity];
		std::vector<std::shared_ptr<ComponentBase>>& globalComponentList = ECSManager::GetInstance()->globalComponentList; 
		for (int i = 0; i < data->associatedComps.size(); i++)
		{
			// Remove component from global list of components
			globalComponentList[data->associatedComps[i].second.first] = nullptr;

			int StartMethodIndex = data->associatedComps[i].second.second.StartMethodIndex;
			int UpdateMethodIndex = data->associatedComps[i].second.second.UpdateMethodIndex;
			int DestroyMethodIndex = data->associatedComps[i].second.second.DestroyMethodIndex;

			// removing the lifecycle calls (Start), if any
			if (StartMethodIndex != -1)
				OnStartCalls[StartMethodIndex] = nullptr;

			// removing the lifecycle calls (Update), if any
			if (UpdateMethodIndex != -1)
			{
				OnUpdateCalls[UpdateMethodIndex].first = nullptr;
				OnUpdateCalls[UpdateMethodIndex].second = nullptr;
			}

			// removing the lifecycle calls (OnDestroy), if any
			if (DestroyMethodIndex != -1)
			{
				std::function<void()> OnDestroyCall = OnDestroyCalls[data->associatedComps[i].second.second.DestroyMethodIndex];
				OnDestroyCalls[DestroyMethodIndex] = nullptr;
				if(ECSManager::isUnderRuntime)
					OnDestroyCall();
			}

		
		}
		//Calling the OnDestroy Call before losing its reference
		std::string str = "Successfully Removed ";
		str.append(typeid(T1).name());
		std::cout << str << std::endl;

		data->associatedComps.clear();
		EntityBase* const abc = entity;
		dir.erase(abc);
	}
	entity->OnDestroyed();
	delete entity;
}
```
### Component Handling
```cpp
template<typename T, typename... Args>
inline std::pair<bool, std::shared_ptr<T>> ECSManager::RegisterComponent(EntityBase* entity, Args&&... args)
{	
	// Brief check for an Invalid Entity
	if (!entity)
	{
		throw std::runtime_error("Entity is null");
		return {false , nullptr};
	}
	else // If we have a valid entity
	{
		auto& dir = ECSManager::GetInstance()->entityDir;
		if (dir.find(entity) == dir.end())
		{
			throw std::runtime_error("Entity does not exist");
		}
		else
		{	// The entity class shouldn't be an abstract one
			if (std::is_abstract<T>())
			{
				throw std::runtime_error("Component can't be an abstract base class");
			}

			// The entity class should be dervied from the component base class
			else if(!std::is_base_of<ComponentBase , T>())
			{	
				std::string str = typeid(T).name();
				str.append(" is not inheriting ComponentBase class");
				throw std::runtime_error(str);
			}
			else
			{	
				std::shared_ptr<EntityData> data = dir[entity];
				std::vector<std::shared_ptr<ComponentBase>>& globalComponentList = ECSManager::GetInstance()->globalComponentList;

				for (auto i : data->associatedComps) // Check whether the component already exists or not
				{
					if (i.first == typeid(T).hash_code())
					{
						std::cout << "Component " << typeid(T).name() << " already exists" << std::endl;
						return { false , std::dynamic_pointer_cast<T>(globalComponentList[i.second.first]) };	
					}
				}
				// If not present then create a component for registration
				std::shared_ptr<T> comp = std::make_shared<T>(entity, std::forward<Args>(args)...);
				globalComponentList.push_back(comp);	


				//--------- ADDING REQUIRED FUNCTION CALLS ----------
				int StartMethodIndex = -1, UpdateMethodIndex = -1, destroyMethodIndex = -1;

				if (isUnderRuntime)
				{	
					comp->Start();
				}
				else
				{
					OnStartCalls.push_back([comp]()
					{
						comp->Start();
					});
					StartMethodIndex = OnStartCalls.size() - 1;
				}

				// If components are allowed to tick then only register their update function for the runtime
				if (comp->GetCanTick())
				{	
					std::pair<std::function<void(const float&) >, EntityBase*> pair = std::make_pair(nullptr,nullptr);
					pair.first = [comp](const float& deltaTime)
						{
							comp->Update(deltaTime);
						};

					pair.second = entity;

					OnUpdateCalls.push_back(pair);
					UpdateMethodIndex = OnUpdateCalls.size() - 1;
				}

				OnDestroyCalls.push_back([comp]()
				{
					comp->OnDestroy();
				});
				destroyMethodIndex = OnDestroyCalls.size() - 1;

				data->associatedComps.push_back({ typeid(T).hash_code() , std::make_pair(globalComponentList.size() - 1, ComponentLifecycleData(StartMethodIndex , UpdateMethodIndex, destroyMethodIndex))
			});
				std::cout << "Component Registered " << typeid(T).name() << std::endl;
				return {true , comp};
			}
		}
	}
}
```
The above function is responsible for the Component Registration for an Entity. The Component Registration function returns ```(<the status of Registration> , <Component*>)``` and highlights the necessary invalidation checks. Once all the invalidation checks are passed then we can simply:
1. Create that component
2. Register its runtime (if any)
3. return it

###
Similarly unregistering an entity follows the reverse approach...

```cpp
template<typename T>
inline bool ECSManager::UnregisterComponent(EntityBase* entity)
{	
	if (!entity)
	{
		throw std::runtime_error("Entity is null");
		return false;
	}
	else
	{
		auto& dir = ECSManager::GetInstance()->entityDir;
		if (dir.find(entity) == dir.end())
		{
			throw std::runtime_error("Entity does not exist");
		}
		else
		{
			if (std::is_abstract<T>())
			{
				throw std::runtime_error("Component can't be an abstract base class");
			}
			else if (!std::is_base_of<ComponentBase, T>())
			{
				std::string str = typeid(T).name();
				str.append(" is not inheriting ComponentBase class");
				throw std::runtime_error(str);
			}
			else
			{	
				std::shared_ptr<EntityData> data = dir[entity];
				std::vector<std::shared_ptr<ComponentBase>>& globalComponentList = ECSManager::GetInstance()->globalComponentList;
				for (int i = 0; i < data->associatedComps.size(); i++)
				{
					if (data->associatedComps[i].first == typeid(T).hash_code())
					{
						// Remove component from global list of components
						globalComponentList[data->associatedComps[i].second.first] = nullptr;

						int StartMethodIndex = data->associatedComps[i].second.second.StartMethodIndex;
						int UpdateMethodIndex = data->associatedComps[i].second.second.UpdateMethodIndex;
						int DestroyMethodIndex = data->associatedComps[i].second.second.DestroyMethodIndex;

						// removing the lifecycle calls (Start), if any
						if (StartMethodIndex != -1)
							OnStartCalls[StartMethodIndex] = nullptr;

						// removing the lifecycle calls (Update), if any
						if (UpdateMethodIndex != -1)
						{
							OnUpdateCalls[UpdateMethodIndex] .first= nullptr;
							OnUpdateCalls[UpdateMethodIndex] .second= nullptr;
						}

						// removing the lifecycle calls (OnDestroy), if any
						if (DestroyMethodIndex != -1)
						{
							std::function<void()> OnDestroyCall = OnDestroyCalls[data->associatedComps[i].second.second.DestroyMethodIndex];
							OnDestroyCalls[DestroyMethodIndex] = nullptr;
							OnDestroyCall();
						}

						// Remove component data from the entity dir 
						data->associatedComps.erase(data->associatedComps.begin() + i);

						//Calling the OnDestroy Call before losing its reference
						std::string str = "Successfully Removed ";
						str.append(typeid(T));
						std::cout << str << std::endl; 
						return true;
					}
				}

				std::cout << "Component not found" << std::endl; 
				return false;
			}
		}
	}
}

```
The approach here is also similar. The Component Unregistration function returns the status of unregistration. We also follow the same pattern of passing invalidation checks. Once we are pass that we:
1. We remove that component from the global list of components
2. Remove the lifecycle calls
3. Remove the component data associated with the entity ```EntityData```

###
Getting the component is also trivial 
```cpp
template<typename T>
inline std::shared_ptr<T> ECSManager::GetComponent(EntityBase* entity)
{
	if (!entity)
	{
		throw std::runtime_error("Entity is null");
		return nullptr;
	}
	else
	{
		auto& dir = ECSManager::GetInstance()->entityDir;
		if (dir.find(entity) == dir.end())
		{
			throw std::runtime_error("Entity does not exist");
		}
		else
		{
			if (std::is_abstract<T>())
			{
				throw std::runtime_error("Component can't be an abstract base class");
			}
			else if (!std::is_base_of<ComponentBase, T>())
			{
				std::string str = typeid(T).name();
				str.append(" is not inheriting ComponentBase class");
				throw std::runtime_error(str);
			}
			else
			{
				std::shared_ptr<EntityData> data = dir[entity];
				std::vector<std::shared_ptr<ComponentBase>>& globalComponentList = ECSManager::GetInstance()->globalComponentList;
				for (int i = 0; i < data->associatedComps.size(); i++)
				{
					if (data->associatedComps[i].first == typeid(T).hash_code())
					{
						// Get the required component from the component list
						return std::dynamic_pointer_cast<T>(globalComponentList[data->associatedComps[i].second.first]);
					}
				}

				return nullptr;
			}
		}
	}
}
```

## Working with ECS
You can check the ```Scene.cpp``` which contains the info on the creating entities and attaching components. However if you want a better view, proceed below:

### Creating a player and adding Behaviour

```cpp
void CreatePlayer()
{	// Creating an Entity named player
	Entity* player = nullptr;
	player = ECSManager::CreateEntity<Entity>(); 
	// Adding a sprite to display on the player's position. The Parameters are as follows : (<Image_path> , <Width X Height>)
	player->TryAddComponent<Sprite>("./Images/Player.png", Vector2{100.0f, 100.0f});
	// Basic Movement Component
	player->TryAddComponent<MovementComponent>();
        // Basic Shooting Component
	player->TryAddComponent<ShooterComponent>();
}
```
Sample Code of Shooter Component which fires bullets upon spacebar pressed

```cpp
#include "ShooterComponent.h"
#include <iostream>
#include <BulletFactory.h>
#include <EntityBase.h>
#include <raylib.h>
ShooterComponent::ShooterComponent(EntityBase* attachedEntity) : ComponentBase(attachedEntity) , shootKeyCode(KEY_SPACE)
{
	std::cout << "ShooterComponent Created" << std::endl;
	canTick = true;
}

// This will execute before the runtime starts
void ShooterComponent::Start() 
{

}

// This will execute every frame
void ShooterComponent::Update(const float& deltaTime)
{
	if (IsKeyPressed(KEY_SPACE))
	{
		Fire();
	}
}

// This will execute once the game ends
void ShooterComponent::OnDestroy()
{

}

// Simple firing logic with bullet pooling
void ShooterComponent::Fire()
{
   ///Your Shooting logic...
}
```
HPP:
```cpp
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
	void Fire();
};


```

