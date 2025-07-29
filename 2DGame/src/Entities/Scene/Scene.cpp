#include "Scene.h"
#include <CameraComponent.h>
#include <ECSManager.h>
#include <BulletFactory.h>
#include <Sprite.h>
#include <MovementComponent.h>
#include <ShooterComponent.h>

Scene::Scene()
{
	this->Init();
}

void Scene::Init()
{	
	
	ECSManager::CreateInstance();
	BulletFactory::EnsureCreation();
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
	BulletFactory::Cleanup(); // Release an bullet resources
}


Scene::~Scene()
{
	this->Unload();
}

//---------- Other Functions --------------
void Scene::LoadPlayer()
{
	player = ECSManager::CreateEntity<Entity>();
	player->TryAddComponent<Sprite>("./Images/Player.png", Vector2{100.0f, 100.0f});
	player->TryAddComponent<MovementComponent>();
	player->TryAddComponent<ShooterComponent>();
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


