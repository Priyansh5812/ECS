#include "TextureHandler.h"


std::unordered_map<std::string, Texture> TextureHandler::reg;

Texture TextureHandler::GetTexture(std::string TexturePath)
{
	if (reg.contains(TexturePath))
	{
		return reg[TexturePath];
	}
	else
	{
		Texture texture = LoadTexture(&TexturePath[0]);
		if (IsTextureValid(texture))
		{
			reg[TexturePath] = texture;
		}

		return texture;
	}
}
