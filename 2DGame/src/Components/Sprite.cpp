#include "Sprite.h"
#include <raylib.h>
#include <EntityBase.h>

Sprite::Sprite(EntityBase* attachedEntity) : ComponentBase(attachedEntity)
{
	canTick = true;
}

void Sprite::Start()
{


}

void Sprite::Update(const float& deltaTime)	
{	
	Draw();
}


void Sprite::Draw()
{
	Vector2 pos = { this->attachedEntity->GetPosition().x ,this->attachedEntity->GetPosition().y };
	Vector2 toScreenSpace = Vector2Add(pos, { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	
	switch (sprite)
	{
		case DefaultSprite::CIRCLE:	
			DrawCircleV(toScreenSpace, 20.0f, RED);
			break;
		case DefaultSprite::RECTANGLE:
			DrawRectangleV(toScreenSpace, { 10.0f , 20.0f }, BLUE);
			break;
		default:
			DrawCircleV(toScreenSpace, 20.0f, PINK);
			break;
	
	}
}

Vector2 Sprite::GetArea()
{	
	return { 20.0f , 20.0f };
}

void Sprite::SetDefaultSpriteBehaviour(DefaultSprite type)
{
	this->sprite = type;
}

void Sprite::OnDestroy()
{
	std::cout << "Destroyed Sprite Component" << std::endl;
}
