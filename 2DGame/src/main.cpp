#include <raylib.h>
#include <iostream>
#include <Constants.h>
#include <ECSManager.h>
#include <Scene.h>

int main()
{	
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Test");
	SetTargetFPS(60);
	//--- Scene Initialization ---
	Scene* scene = new Scene();
	
	while (!WindowShouldClose())
	{
		scene->Update();
	}

	CloseWindow();
	delete scene;
	return 0;
}
