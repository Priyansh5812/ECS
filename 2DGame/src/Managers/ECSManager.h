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

class Entity;
class ComponentBase;

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
		std::vector<std::pair<size_t, int>> associatedComps;
};



class ECSManager
{	

public:
	ECSManager();
	static void CreateInstance();
	static const std::unique_ptr<ECSManager>& GetConstInstance();
	static const int& RegisterEntity(Entity* entity);

	template<typename T>
	static std::pair<bool , std::shared_ptr<T>> RegisterComponent(Entity* entity);

	template <typename T>
	static bool UnregisterComponent(Entity* entity);

	template<typename T>
	static std::shared_ptr<T> GetComponent(Entity* entity);

	static void Cleanup();
	static void RunStartCalls();
	static void RunUpdateCalls(const float& deltaTime);
	static void RunOnDestroyCalls();

private:
	static int globalIdCounter;
	static std::unique_ptr<ECSManager> m_instance;

private:
	std::vector<std::shared_ptr<ComponentBase>> globalComponentList;
	std::unordered_map<Entity*, EntityData*> entityDir; // Dictionary for storing the entities in (Entity*) x UID , List of (Component HashCode , Global Component List Index)
	// the format of storing lifecycle calls is the (typeId(Component) , Function Pointer)
	static std::vector<std::pair<size_t,std::function<void()>>> OnStartCalls; // Array to store the Start Calls
	static std::vector<std::pair<size_t,std::function<void(const float&)>>> OnUpdateCalls; // Array to store the UpdateCalls
	static std::vector<std::pair<size_t,std::function<void()>>>OnDestroyCalls; // Array to store the Destroy Calls
private:
	
	static std::unique_ptr<ECSManager>& GetInstance();

};



#pragma region Templated Definations
template<typename T>
inline std::pair<bool, std::shared_ptr<T>> ECSManager::RegisterComponent(Entity* entity)
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
				EntityData* data = dir[entity];
				std::vector<std::shared_ptr<ComponentBase>>& globalComponentList = ECSManager::GetInstance()->globalComponentList;
				for (auto i : data->associatedComps) 
				{
					if (i.first == typeid(T).hash_code())
					{
						std::cout << "Component " << typeid(T).name() << " already exists" << std::endl;
						return { false , std::dynamic_pointer_cast<T>(globalComponentList[i.second]) };	
					}
				}

				std::shared_ptr<T> comp = std::make_shared<T>(entity);
				globalComponentList.push_back(comp);	
				data->associatedComps.push_back({ typeid(T).hash_code() , globalComponentList.size() - 1});

				//--------- ADDING REQUIRED FUNCTION CALLS ----------
				OnStartCalls.push_back({typeid(T).hash_code(),[comp]()
					{
						comp->Start();
					}});
				OnUpdateCalls.push_back({typeid(T).hash_code(),[comp](const float& deltaTime)
					{
						comp->Update(deltaTime);
					}});
				OnDestroyCalls.push_back({typeid(T).hash_code(),[comp]()
					{
						comp->OnDestroy();
					}});

				return {true , comp};
			}
		}
	}
}

template<typename T>
inline bool ECSManager::UnregisterComponent(Entity* entity)
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
				EntityData* data = dir[entity];
				std::vector<std::shared_ptr<ComponentBase>>& globalComponentList = ECSManager::GetInstance()->globalComponentList;
				for (int i = 0; i < data->associatedComps.size(); i++)
				{
					if (data->associatedComps[i].first == typeid(T).hash_code())
					{	
						// Remove component from global list of components
						globalComponentList.erase(globalComponentList.begin() + data->associatedComps[i].second); 
						
						// removing the lifecycle calls (Start)
						OnStartCalls.erase(OnStartCalls.begin() + data->associatedComps[i].second);
						
						// removing the lifecycle calls (Update)
						OnUpdateCalls.erase(OnUpdateCalls.begin() + data->associatedComps[i].second);

						// removing the lifecycle calls (OnDestroy)
						std::function<void()> OnDestroyCall = OnDestroyCalls[data->associatedComps[i].second].second;
						OnDestroyCalls.erase(OnDestroyCalls.begin() + data->associatedComps[i].second);

						// Remove component data from the entity dir
						data->associatedComps.erase(data->associatedComps.begin() + i);

						//Calling the OnDestroy Call before losing its reference
						OnDestroyCall();
						std::cout << "Successfully Removed" << std::endl; 
						return true;
					}
				}

				return false;
			}
		}
	}
}

template<typename T>
inline std::shared_ptr<T> ECSManager::GetComponent(Entity* entity)
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
				EntityData* data = dir[entity];
				std::vector<std::shared_ptr<ComponentBase>>& globalComponentList = ECSManager::GetInstance()->globalComponentList;
				for (int i = 0; i < data->associatedComps.size(); i++)
				{
					if (data->associatedComps[i].first == typeid(T).hash_code())
					{
						// Remove component from global list of components
						return std::dynamic_pointer_cast<T>(globalComponentList[data->associatedComps[i].second]);
					}
				}

				return nullptr;
			}
		}
	}
}

#pragma endregion
