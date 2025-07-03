#pragma once
#include <unordered_map>
#include <vector>
#include <Constants.h>
#include <utility>
#include <stdexcept>
#include <iostream>
#include <string>
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
	static void CreateInstance();
	static const ECSManager* GetConstInstance();
	static const int& RegisterEntity(Entity* entity);
	template<typename T>
	static std::pair<bool , T*> RegisterComponent(Entity* entity);
	static void Cleanup();


private:
	static int globalIdCounter;
	static ECSManager* m_instance;
	std::vector<ComponentBase*> globalComponentList;
	std::unordered_map<Entity*, EntityData> entityDir; // Dictionary for storing the entities in (Entity*) x UID , List of (Component HashCode , Global Component List Index, )
	ECSManager();
	ECSManager(const ECSManager& copy) = delete;	
	static ECSManager* GetInstance();
	
	

	


};



#pragma region Templated Definations
template<typename T>
inline std::pair<bool, T*> ECSManager::RegisterComponent(Entity* entity)
{
	if (!entity)
	{
		throw std::runtime_error("Entity is null");
		return { false , nullptr};
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
			    EntityData& data = dir[entity];
				std::vector<ComponentBase*>& globalComponentList = ECSManager::GetInstance()->globalComponentList;
				for (auto i : data.associatedComps)
				{
					if (i.first == typeid(T).hash_code())
					{
						std::cout << "Component " << typeid(T).name() << "already exists" << std::endl;
						return { false , dynamic_cast<T*>(globalComponentList[i.second]) };
					}
				}
				T* comp = new T(entity);
				globalComponentList.push_back(comp);	
				data.associatedComps.push_back({ typeid(T).hash_code() , globalComponentList.size() - 1});
				return { true , comp};
			}
		}



	}

}
#pragma endregion
