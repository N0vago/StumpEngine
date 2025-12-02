#ifndef ST_TEXTURE_H
#define ST_TEXTURE_H

#include <glad/glad.h>
#include <stb/stb_image.h>
#include "Shader.h"
class Texture
{
public:
	GLuint ID;

	const char* type;

	GLuint unit;

	Texture(const char* image, const char* textureType, GLuint slot);

	void TextureUnit(Shader& shader, const char* uniform, GLuint unit);

	void Bind();

	void Unbind();

	void Delete();
};

#endif

