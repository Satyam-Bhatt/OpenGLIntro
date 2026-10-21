#include "TextureLoader.h"

TextureLoader::TextureLoader(const std::string & texturePath, TextureProperties texProp)
{}

TextureLoader::TextureLoader()
{}

void TextureLoader::Use()
{
	glBindTexture(GL_TEXTURE_2D, ID);
}

