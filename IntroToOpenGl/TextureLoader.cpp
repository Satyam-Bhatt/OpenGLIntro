#include "TextureLoader.h"

TextureLoader::TextureLoader(const std::string & texturePath, TextureProperties texProp)
{
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	if(texProp.textureWrappingS == TextureWrapping::Repeat)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	else if(texProp.textureWrappingS == TextureWrapping::Mirror)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	else if(texProp.textureWrappingS == TextureWrapping::Clamp)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	else if(texProp.textureWrappingS == TextureWrapping::Border)
	{
		float col[4] = { texProp.borderColor.x, texProp.borderColor.y, texProp.borderColor.z, texProp.borderColor.w };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, col); // TODO: How to
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	}

	if (texProp.textureWrappingT == TextureWrapping::Repeat)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	else if (texProp.textureWrappingT == TextureWrapping::Mirror)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	else if (texProp.textureWrappingT == TextureWrapping::Clamp)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	else if (texProp.textureWrappingT == TextureWrapping::Border)
	{
		float col[4] = { texProp.borderColor.x, texProp.borderColor.y, texProp.borderColor.z, texProp.borderColor.w };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, col); // TODO: How to
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	}

	if(texProp.textureMinFiltering == TextureMinFiltering::Linear)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	else if (texProp.textureMinFiltering == TextureMinFiltering::Nearest)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	else if (texProp.mipMapBias && texProp.textureMinFiltering == TextureMinFiltering::Nearest_MipNearest)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	else if (texProp.mipMapBias && texProp.textureMinFiltering == TextureMinFiltering::Linear_MipNearest)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	else if (texProp.mipMapBias && texProp.textureMinFiltering == TextureMinFiltering::Nearest_MipLinear)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	else if (texProp.mipMapBias && texProp.textureMinFiltering == TextureMinFiltering::Linear_MipLinear)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	if(texProp.textureMagFiltering == TextureMagFiltering::Linear)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	else if(texProp.textureMagFiltering == TextureMagFiltering::Nearest)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nChannels;
	unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nChannels, 0);

	if (data)
	{
		if(texProp.colorType == ColorType::RGB)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		if(texProp.generateMipmaps) glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	
	stbi_image_free(data);
}

TextureLoader::TextureLoader()
{}

void TextureLoader::Bind()
{
	glBindTexture(GL_TEXTURE_2D, ID);
}

void TextureLoader::Unbind()
{
	if (ID != 0) {
		glDeleteTextures(1, &ID); // TODO: Can I have more textures?
		ID = 0;
	}
}

