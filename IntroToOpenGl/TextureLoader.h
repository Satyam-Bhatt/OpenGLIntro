#pragma once

#include <iostream>
#include <stdio.h>
#include <glad.h>
#include <GLFW/glfw3.h>
#include "Vector.h"
#include "stb_image.h"

#ifndef  TEXTURELOADER_H
#define TEXTURELOADER_H

enum TextureWrapping
{
	Repeat,
	Mirror,
	Clamp,
	Border
};

enum TextureMinFiltering
{
	Nearest,
	Linear,
	Nearest_MipNearest,
	Linear_MipNearest,
	Nearest_MipLinear,
	Linear_MipLinear
};

enum TextureMagFiltering
{
	Nearest,
	Linear,
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
	TextureMinFiltering textureMinFiltering = TextureMinFiltering::Linear_MipLinear;
	TextureMagFiltering textureMagFiltering = TextureMagFiltering::Linear;
	ColorType colorType = ColorType::RGBA;
	Vector::Vector4 borderColor = Vector::Vector4(1,1,1,1);

	TextureProperties()
	{
		generateMipmaps = true;
		mipMapBias = 0;
		height = 0;
		width = 0;
		textureWrappingS = TextureWrapping::Repeat;
		textureWrappingT = TextureWrapping::Repeat;
		colorType = ColorType::RGBA;
		borderColor = Vector::Vector4(1, 1, 1, 1);
		if (generateMipmaps)
			textureMinFiltering = TextureMinFiltering::Linear_MipLinear;
		else
			textureMinFiltering = TextureMinFiltering::Linear;

		textureMagFiltering = TextureMagFiltering::Linear;
	}
};

class TextureLoader
{
public:
	unsigned int ID = 0;

	TextureLoader();

	TextureLoader(const std::string& texturePath, TextureProperties texProp); // TODO: Have something default

	void Use();
	void Dispose();
};

#endif