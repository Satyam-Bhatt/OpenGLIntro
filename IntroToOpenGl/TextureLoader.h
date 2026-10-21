#pragma once

#include <iostream>
#include <stdio.h>
#include <glad.h>
#include <GLFW/glfw3.h>

#ifndef  TEXTURELOADER_H
#define TEXTURELOADER_H

enum TextureWrapping
{
	Repeat,
	Mirror,
	Clamp,
	Border
};

enum TextureFiltering
{
	Nearest,
	Linear,
	Nearest_MipNearest,
	Linear_MipNearest,
	Nearest_MipLinear,
	Linear_MipLinear
};

enum ColorType
{
	RGB,
	RGBA
};

struct TextureProperties
{
	bool generateMipmaps = true;
	int mipMapBias = 0;
	int height = 0, width = 0;
	TextureWrapping textureWrappingS = TextureWrapping::Repeat;
	TextureWrapping textureWrappingT = TextureWrapping::Repeat;
	TextureFiltering textureFiltering = TextureFiltering::Linear_MipLinear;
	ColorType colorType = ColorType::RGBA;

	TextureProperties()
	{
		generateMipmaps = true;
		mipMapBias = 0;
		height = 0;
		width = 0;
		textureWrappingS = TextureWrapping::Repeat;
		textureWrappingT = TextureWrapping::Repeat;
		colorType = ColorType::RGBA;
		if (generateMipmaps)
			textureFiltering = TextureFiltering::Linear_MipLinear;
		else
			textureFiltering = TextureFiltering::Linear;
	}
};

class TextureLoader
{
public:
	unsigned int ID;

	TextureLoader();

	TextureLoader(const std::string& texturePath, TextureProperties texProp);

	void Use();
};



#endif