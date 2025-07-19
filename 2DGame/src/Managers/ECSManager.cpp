#include "ECSManager.h"
#include "EntityBase.h"
#include <ComponentBase.h>


int ECSManager::globalIdCounter = 0;
std::unique_ptr<ECSManager> ECSManager::m_instance;
bool ECSManager::isUnderRuntime = false;

std::vector<std::function<void()>> ECSManager::OnStartCalls;
std::vector<std::pair<std::function<void(const float&)> , EntityBase*>> ECSManager::OnUpdateCalls;	
std::vector<std::function<void()>> ECSManager::OnDestroyCalls;

//Maintaining Cache Locality from the start
ECSManager::ECSManager()
{
	this->globalComponentList.reserve(20); // will expand on demand
}


//Ensures creating Instance
void ECSManager::CreateInstance()
{
	m_instance = std::make_unique<ECSManager>();
}


//Will be used for private scope
std::unique_ptr<ECSManager>& ECSManager::GetInstance()
{	
	return m_instance;
}


// Will be used for public scope
const std::unique_ptr<ECSManager>& ECSManager::GetConstInstance()
{	
	return GetInstance();
}

// Cleanup what has been allocated
void ECSManager::Cleanup()
{	

	isUnderRuntime = false;
	std::vector<EntityBase*> toDestroy;
	toDestroy.reserve(ECSManager::GetInstance()->entityDir.size());
	for (auto& i : ECSManager::GetInstance()->entityDir)
	{
		toDestroy.push_back(i.first);
	}

	while (toDestroy.size() > 0)
	{
		EntityBase* en = toDestroy.back();
		ECSManager::DestroyEntity(en);
		toDestroy.pop_back();
	}

	

	OnStartCalls.clear();
	OnUpdateCalls.clear();
	OnDestroyCalls.clear();

}

// Entity Registration
const int& ECSManager::RegisterEntity(EntityBase* entity)
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
		return dir[entity]->entityID;
	}

	// Allocating this on heap due, to Entity data can also be very vast
	//dir[entity] = new EntityData(globalIdCounter++);
	dir[entity] = std::make_shared<EntityData>(globalIdCounter++);

	std::cout << "Entity successfully registered at " << globalIdCounter - 1 << std::endl;

	return globalIdCounter - 1;
}


void ECSManager::RunStartCalls()
{
	isUnderRuntime = true;

	for (int i = 0; i < OnStartCalls.size(); i++)
	{
		if (OnStartCalls[i])
		{
			OnStartCalls[i]();
		}
	}
}

void ECSManager::RunUpdateCalls(const float& deltaTime)
{
	for (int i = 0; i < OnUpdateCalls.size(); i++)
	{
		if (OnUpdateCalls[i].first && OnUpdateCalls[i].second->GetActive())
		{
			OnUpdateCalls[i].first(deltaTime);
		}
	}
}

void ECSManager::RunOnDestroyCalls()
{
	for (int i = 0; i < OnDestroyCalls.size(); i++)
	{
		if (OnDestroyCalls[i])
		{
			OnDestroyCalls[i]();
		}
	}
}




