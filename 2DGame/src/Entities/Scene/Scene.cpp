#include "Scene.h"
#include <CameraComponent.h>
#include <ECSManager.h>
#include <Sprite.h>
#include <MovementComponent.h>
Scene::Scene()
{
	this->Init();
}

void Scene::Init()
{
	ECSManager::CreateInstance();
	LoadBasicEnts();
	ECSManager::RunStartCalls();// Sets the runtime flag to true...
}

void Scene::Update()
{	
	BeginDrawing();
	ClearBackground(LIGHTGRAY);
	ECSManager::RunUpdateCalls(GetFrameTime());
	BeginMode2D(*(this->camera->GetCamera().get()));
	EndMode2D();
	EndDrawing();
	
}

void Scene::Unload()
{
	ECSManager::RunOnDestroyCalls();	
	ECSManager::Cleanup(); // Sets  Runtime Flags to false...
	
}


Scene::~Scene()
{
	this->Unload();
}

//---------- Other Functions --------------
void Scene::LoadPlayer()
{
	player = ECSManager::CreateEntity<Entity>();
	player->TryAddComponent<Sprite>();
	player->TryAddComponent<MovementComponent>();
}

void Scene::LoadCamera()
{
	Entity* en = ECSManager::CreateEntity<Entity>();
	this->camera = en->TryAddComponent<CameraComponent>();
}

void Scene::LoadBasicEnts()
{
	LoadCamera();
	LoadPlayer();
	
}


