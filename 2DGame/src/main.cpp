#include <raylib.h>
#include <iostream>
#include <raymath.h>
#include <Constants.h>
#include <ECSManager.h>
#include <Entity.h>
#include <CustomComponent.h>

int main()
{	
	
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Test");
	Camera2D camera = Camera2D();
	Vector2 playerPos = { 0,0 };
	camera.target = playerPos;
	camera.offset = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
	SetTargetFPS(60);
	ECSManager::CreateInstance();
	Entity* en = new Entity();
	ECSManager::RunStartCalls();
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(LIGHTGRAY);
		BeginMode2D(camera);
		ECSManager::RunUpdateCalls(GetFrameTime());
		EndMode2D();
		EndDrawing();
	}
	ECSManager::RunOnDestroyCalls();
	ECSManager::Cleanup();
	CloseWindow();
	
	return 0;
}

