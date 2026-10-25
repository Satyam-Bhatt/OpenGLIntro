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

enum class TextureMinFiltering
{
	Nearest,
	Linear,
	Nearest_MipNearest,
	Linear_MipNearest,
	Nearest_MipLinear,
	Linear_MipLinear
};

enum class TextureMagFiltering
{
	Nearest,
	Linear,
};

enum ColorType
{
	Auto,
	RGB,
	RGBA
};

struct TextureProperties
{
	bool generateMipmaps = true;
	float mipMapBias = 0;
	int height = 0, width = 0;
	TextureWrapping textureWrappingS = TextureWrapping::Repeat;
	TextureWrapping textureWrappingT = TextureWrapping::Repeat;
	TextureMinFiltering textureMinFiltering = TextureMinFiltering::Linear_MipLinear;
	TextureMagFiltering textureMagFiltering = TextureMagFiltering::Linear;
	ColorType colorType = ColorType::Auto;
	Vector::Vector4 borderColor = Vector::Vector4(1,1,1,1);

	TextureProperties()
	{
		generateMipmaps = true;
		mipMapBias = 0;
		height = 0;
		width = 0;
		textureWrappingS = TextureWrapping::Repeat;
		textureWrappingT = TextureWrapping::Repeat;
		colorType = ColorType::Auto;
		borderColor = Vector::Vector4(1, 1, 1, 1);
		if (generateMipmaps)
			textureMinFiltering = TextureMinFiltering::Linear_MipLinear;
		else
			textureMinFiltering = TextureMinFiltering::Linear;

		textureMagFiltering = TextureMagFiltering::Linear;
	}

	static TextureProperties Pixelated()
	{
		TextureProperties p;
		p.textureMinFiltering = TextureMinFiltering::Nearest;
		p.textureMagFiltering = TextureMagFiltering::Nearest;
		p.generateMipmaps = false;
		return p;
	}
	static TextureProperties UI()
	{
		TextureProperties p;
		p.textureWrappingS = p.textureWrappingT = TextureWrapping::Clamp;
		p.generateMipmaps = false;
		p.textureMinFiltering = TextureMinFiltering::Linear;
		return p;
	}
};

class TextureLoader
{
public:
	unsigned int ID = 0;

	TextureLoader();

	// TextureLoader.h
	TextureLoader(const std::string& texturePath, const TextureProperties& texProp = TextureProperties()); 

	void Bind(unsigned int slot = 0);
	void Unbind();
};

#endif