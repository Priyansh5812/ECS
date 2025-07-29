# Space Invaders via Custom Entity Component System (ECS)
This readme will guide you through the architecture I followed in order to make the ECS.


## The ECS Core
The Entity Component System gets managed by a single class ```ECSManager``` and is divided for Entities and Components separately.

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
Upon successfull creation of an Entity, its ```Entity::OnCreated()``` gets invoked



