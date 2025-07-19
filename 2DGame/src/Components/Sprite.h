#pragma once
#include <ComponentBase.h>
struct Vector2;
enum DefaultSprite
{
	CIRCLE,
	RECTANGLE
};

class Sprite : public ComponentBase
{	
	public:
		Sprite(EntityBase* attachedEntity);
		virtual void Start() override; // Initialization 
		virtual void Update(const float& deltaTime) override; // OnTick
		virtual void OnDestroy() override;// Deinitialization
	public:
		Vector2 GetArea();
		void SetDefaultSpriteBehaviour(DefaultSprite type);
	private:
		void Draw();
	private:
		DefaultSprite sprite = DefaultSprite::CIRCLE;
};

