#include <GL/glew.h>
#include "gfx_texture.h"

// STB Image implementation goes here
#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"


gfx::Texture::Texture(const char *path)
{
	// Creating texture
	glGenTextures(1, &id);
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load image from disk
	int channels;
	stbi_set_flip_vertically_on_load(1);
	u8 *data = stbi_load(path, &width, &height, &channels, 0);
	if(data == NULL)
	{
		Panic("Can't load image %s: %s", path, stbi_failure_reason());
	}

	// Bring CPU image to GPU
	u32 channel = channels == 4 ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, channel, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Free CPU image
	stbi_image_free(data);
}


void gfx::Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void gfx::Texture::Delete()
{
	glDeleteTextures(GL_TEXTURE_2D, &id);
}