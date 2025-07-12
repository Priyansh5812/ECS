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
	DrawCircleV(toScreenSpace, 20.0f, RED);
}

void Sprite::OnDestroy()
{
	std::cout << "Destroyed Sprite Component" << std::endl;
}
