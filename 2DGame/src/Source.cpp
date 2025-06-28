#include <raylib.h>
#include <iostream>
#include <raymath.h>
#include <Constants.h>

void HandleInput(Vector2& pos, float speed , float deltaTime)
{
	
	if (IsKeyDown(KEY_W))
		pos.y -= speed * deltaTime;

	if(IsKeyDown(KEY_A))
		pos.x -= speed * deltaTime;
		
	if(IsKeyDown(KEY_S))
		pos.y += speed * deltaTime;
	
	if(IsKeyDown(KEY_D))
		pos.x += speed * deltaTime;


}


void HandleCameraInput(Vector2& offset, float speed, float deltaTime)
{

	if (IsKeyDown(KEY_W))
		offset.y += speed * deltaTime;

	if (IsKeyDown(KEY_A))
		offset.x += speed * deltaTime;

	if (IsKeyDown(KEY_S))
		offset.y -= speed * deltaTime;

	if (IsKeyDown(KEY_D))
		offset.x -= speed * deltaTime;

}

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

	while (!WindowShouldClose())
	{
		HandleInput(playerPos, 50.0f ,GetFrameTime());
		HandleCameraInput(camera.offset, 50.0f ,GetFrameTime());
		BeginDrawing();
		ClearBackground(LIGHTGRAY);
		BeginMode2D(camera);
		DrawCircleV(playerPos, 20.0f, RED);
		DrawRectangleV({ -50 , -150 }, {100,100} , BLUE);
		EndMode2D();
		EndDrawing();
	}

	CloseWindow();
	return 0;
}