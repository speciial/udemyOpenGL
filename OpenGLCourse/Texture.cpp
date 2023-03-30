#include "Texture.h"

Texture::Texture() : 
	textureID(0), width(0), height(0), bitDepth(0), filePath("")
{
}

Texture::Texture(const char* filePath) :
	textureID(0), width(0), height(0), bitDepth(0), filePath(filePath)
{
}

Texture::~Texture()
{
	ClearTexture();
}

void Texture::LoadTexture()
{
	unsigned char* textureData = stbi_load(filePath, &width, &height, &bitDepth, 0);
	if (!textureData)
	{
		printf("Failed To Load Image: %s\n", filePath);
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// TODO(christian): what happens if bitDepth does not equal 3 or 4? Can that even happen?
	if (bitDepth == 3) 
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	} 
	else if (bitDepth == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
	}
	
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(textureData);
}

void Texture::UseTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	filePath = "";
}
