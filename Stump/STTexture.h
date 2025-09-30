#ifndef ST_TEXTURE_H
#define ST_TEXTURE_H

#include <glad/glad.h>
#include <stb/stb_image.h>
#include "STShader.h"
class STTexture
{
public:
	GLuint ID;

	const char* type;

	GLuint unit;

	STTexture(const char* image, const char* textureType, GLuint slot);

	void texUnit(STShader& shader, const char* uniform, GLuint unit);

	void Bind();

	void Unbind();

	void Delete();
};

#endif

