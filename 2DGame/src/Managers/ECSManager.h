#pragma once
#include <unordered_map>
#include <vector>
#include <Constants.h>
#include <utility>
#include <stdexcept>
#include <iostream>
#include <string>
#include <functional>
#include <memory>

class EntityBase;
class Entity;
class ComponentBase;


struct ComponentLifecycleData
{	
public:
	ComponentLifecycleData(int a, int b, int c)
	{
		StartMethodIndex = a;
		UpdateMethodIndex = b;
		DestroyMethodIndex = c;
	}
	int StartMethodIndex;
	int UpdateMethodIndex;
	int DestroyMethodIndex;
};

struct EntityData
{	
	public:
		EntityData()
		{
			//noop
		}

		EntityData(int entityID)
		{
			this->entityID = entityID;
		}
		int entityID;
		std::vector<std::pair<size_t, std::pair<int , ComponentLifecycleData>>> associatedComps;
		
};





class ECSManager
{	

public:
	ECSManager();
	static void CreateInstance();
	static const std::unique_ptr<ECSManager>& GetConstInstance();
	static const int& RegisterEntity(EntityBase* entity);

	template<typename T>
	static T*& CreateEntity();

	template<typename T1>
	static void DestroyEntity(T1* entity);

	template<typename T>
	static std::pair<bool , std::shared_ptr<T>> RegisterComponent(EntityBase* entity);

	template <typename T>
	static bool UnregisterComponent(EntityBase* entity);

	template<typename T>
	static std::shared_ptr<T> GetComponent(EntityBase* entity);
	
	static void Cleanup();
	static void RunStartCalls();
	static void RunUpdateCalls(const float& deltaTime);
	static void RunOnDestroyCalls();

private:
	static int globalIdCounter;
	static std::unique_ptr<ECSManager> m_instance;
	static bool isUnderRuntime;

private:
	std::vector<std::shared_ptr<ComponentBase>> globalComponentList;
	std::unordered_map<EntityBase*, std::shared_ptr<EntityData>> entityDir; // Dictionary for storing the entities in (EntityBase*) , List of (Component HashCode , (Global Component List Index , LifeCycle Method Data))
	
	// the format of storing lifecycle calls is the (typeId(Component) , Function Pointer)
	static std::vector<std::function<void()>> OnStartCalls; // Array to store the Start Calls
	static std::vector<std::pair<std::function<void(const float&) > , EntityBase*>> OnUpdateCalls; // Array to store the UpdateCalls
	static std::vector<std::function<void()>>OnDestroyCalls; // Array to store the Destroy Calls

private:
	
	static std::unique_ptr<ECSManager>& GetInstance();
	
	

};



#pragma region Templated Definations
template<typename T>
inline T*& ECSManager::CreateEntity()
{	
	if (std::is_abstract<T>())
	{
		throw std::runtime_error("Entity can't be an abstract class");
	}

	if (!std::is_base_of<EntityBase, T>())
	{
		std::string str = typeid(T).name();
		str.append(" is not inheriting EntityBase class");
		throw std::runtime_error(str);
	}
	
	T* ptr = new T();
	ptr->OnCreated();
	return ptr;
}



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










template<typename T>
inline std::pair<bool, std::shared_ptr<T>> ECSManager::RegisterComponent(EntityBase* entity)
{	
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
		{
			if (std::is_abstract<T>())
			{
				throw std::runtime_error("Component can't be an abstract base class");
			}
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
				for (auto i : data->associatedComps) 
				{
					if (i.first == typeid(T).hash_code())
					{
						std::cout << "Component " << typeid(T).name() << " already exists" << std::endl;
						return { false , std::dynamic_pointer_cast<T>(globalComponentList[i.second.first]) };	
					}
				}

				std::shared_ptr<T> comp = std::make_shared<T>(entity);
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
						// Remove component from global list of components
						return std::dynamic_pointer_cast<T>(globalComponentList[data->associatedComps[i].second.first]);
					}
				}

				return nullptr;
			}
		}
	}
}

#pragma endregion
