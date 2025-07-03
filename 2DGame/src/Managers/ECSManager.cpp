#include "ECSManager.h"
#include "Entity.h"
#include <ComponentBase.h>




int ECSManager::globalIdCounter = 0;
ECSManager* ECSManager::m_instance = nullptr;


//Maintaining Cache Locality from the start
ECSManager::ECSManager()
{
	this->globalComponentList.reserve(20); // will expand on demand
}


//Ensures creating Instance
void ECSManager::CreateInstance()
{
	m_instance = new ECSManager();
}


//Will be used for private scope
ECSManager* ECSManager::GetInstance()
{	
	return m_instance;
}


// Will be used for public scope
const ECSManager* ECSManager::GetConstInstance()
{	
	return GetInstance();
}

// Cleanup what has been allocated
void ECSManager::Cleanup()
{	
	auto& entityDir = ECSManager::GetInstance()->entityDir;
	auto& comps = ECSManager::GetInstance()->globalComponentList;
	for (auto i : entityDir)
	{
		i.second.associatedComps.clear();
		delete i.first;
	}

	while (!comps.empty())
	{
		ComponentBase* comp = comps.back();
		comps.pop_back();
		delete comp;
	}

	delete m_instance;
	m_instance = nullptr;

}

const int& ECSManager::RegisterEntity(Entity* entity)
{	
	if (!entity)
	{
		std::cout << "Entity can't be a null for registration" << std::endl;
		return -1;
	}

	auto& dir = ECSManager::GetInstance()->entityDir;

	// Checks if similar entity already exists in the Directory
	if (dir.find(entity) != dir.end())
	{
		std::cout << "Entity already exists" << std::endl;
		return dir[entity].entityID;
	}

	// Allocating this on heap due, to Entity data can also be very vast
	dir[entity] = *(new EntityData(globalIdCounter++));
	
	std::cout << "Entity successfully registered at " << globalIdCounter - 1 << std::endl;

	return globalIdCounter - 1;
}





