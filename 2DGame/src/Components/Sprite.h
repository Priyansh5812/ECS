#pragma once
#include <ComponentBase.h>
#include <string>
#include <raylib.h>

struct Vector2;
enum DefaultSprite
{
	CIRCLE,
	RECTANGLE,
	CUSTOM
};

class Sprite : public ComponentBase
{	
	public:
		Sprite(EntityBase* attachedEntity, std::string spritePath, Vector2 resolution);
		virtual void Start() override; // Initialization 
		virtual void Update(const float& deltaTime) override; // OnTick
		virtual void OnDestroy() override;// Deinitialization
	public:
		Vector2 GetArea();
	private:
		void Load();
		void Draw();

	private:
		bool isSpriteLoaded = false;
		std::string texturePath;
		Texture2D texture;
		Vector2 textureRes;
};

