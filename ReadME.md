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


