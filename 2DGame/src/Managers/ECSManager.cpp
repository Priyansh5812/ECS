#include "ECSManager.h"
#include "Entity.h"
#include <ComponentBase.h>

int ECSManager::globalIdCounter = 0;
std::unique_ptr<ECSManager> ECSManager::m_instance;

std::vector<std::pair<size_t, std::function<void()>>> ECSManager::OnStartCalls;
std::vector<std::pair<size_t , std::function<void(const float&)>>> ECSManager::OnUpdateCalls;	
std::vector<std::pair<size_t, std::function<void()>>> ECSManager::OnDestroyCalls;

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
	for (auto i : ECSManager::GetInstance()->entityDir)
	{	
		delete i.second;
		delete i.first;
	}

	OnStartCalls.clear();
	OnUpdateCalls.clear();
	OnDestroyCalls.clear();
}

// Entity Registration
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
		return dir[entity]->entityID;
	}

	// Allocating this on heap due, to Entity data can also be very vast
	dir[entity] = new EntityData(globalIdCounter++);
	
	std::cout << "Entity successfully registered at " << globalIdCounter - 1 << std::endl;

	return globalIdCounter - 1;
}



void ECSManager::RunStartCalls()
{

	for (auto i : OnStartCalls)
	{
		if (i.second)
			i.second();
	}
}

void ECSManager::RunUpdateCalls(const float& deltaTime)
{
	for (auto i : OnUpdateCalls)
	{
		if (i.second)
			i.second(deltaTime);
	}
}

void ECSManager::RunOnDestroyCalls()
{
	for (auto i : OnDestroyCalls)
	{
		if (i.second)
			i.second();
	}
}

