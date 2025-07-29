#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include <raylib.h>


class TextureHandler
{	
	
	public:
		static Texture GetTexture(std::string TexturePath);


	private:
		static std::unordered_map<std::string, Texture> reg;
};

