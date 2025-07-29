#include "Sprite.h"
#include <EntityBase.h>
#include <TextureHandler.h>
Sprite::Sprite(EntityBase* attachedEntity, std::string spritePath, Vector2 resolution) : ComponentBase(attachedEntity)
{
	canTick = true;
	this->texturePath = spritePath;
	this->textureRes = resolution;
}

void Sprite::Start()
{
	Load();
}

void Sprite::Update(const float& deltaTime)	
{	
	Draw();
}


void Sprite::Load()
{	
	try
	{	
		texture = TextureHandler::GetTexture(texturePath);
		if (IsTextureValid(texture))
		{
			texture.width = textureRes.x;
			
			texture.height = textureRes.y;
		}
	}
	catch (std::exception e)
	{
		std::cout << "Image load via path" << texturePath << " failed !" << std::endl;
	}
}

void Sprite::Draw()
{
	Vector2 pos = { this->attachedEntity->GetPosition().x ,this->attachedEntity->GetPosition().y };
	Vector2 toScreenSpace = Vector2Add(pos, { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	if (IsTextureValid(texture))
	{	
		DrawTextureV(texture, Vector2Add(toScreenSpace, textureRes / -2.0f), WHITE);
	}
	else
	{
		DrawCircleV(toScreenSpace, 20.0f, RED);
	}
}

Vector2 Sprite::GetArea()
{
	Vector2 area;
	if (IsTextureValid(texture))
		area = { (float)texture.width, (float)texture.height };
	else
		area = Vector2One() * 20.0f;
	return area;
}


void Sprite::OnDestroy()
{
	std::cout << "Destroyed Sprite Component" << std::endl;
	if(IsTextureValid(texture))
		UnloadTexture(texture);
}
