#include "Scene.h"
#include <CameraComponent.h>
#include <ECSManager.h>
#include <Sprite.h>
#include <InputComponent.h>
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
	DrawCircleV({ 0,0 }, 20.0f, BLUE);
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
	player->TryAddComponent<InputComponent>();
}

void Scene::LoadCamera()
{
	Entity* en = ECSManager::CreateEntity<Entity>();
	this->camera = en->TryAddComponent<CameraComponent>();
	
	CameraInitData data;
	data.targetEntity = player;
	//data.screenOffset = { SCREEN_WIDTH / 2.0f , SCREEN_HEIGHT / 2.0f };
	data.screenOffset = { SCREEN_WIDTH / 2.0f , SCREEN_HEIGHT / 2.0f };
	data.rotation = 0.0f;
	data.zoom = 0.0f;
	
	this->camera->ConfigureCamera(data);
}

void Scene::LoadBasicEnts()
{
	LoadPlayer();
	LoadCamera();
	
}


