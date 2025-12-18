#ifndef ST_TEXTURE_H
#define ST_TEXTURE_H

#include <glad/glad.h>
#include <stb/stb_image.h>
namespace Rendering {
	enum class TextureType {
		Diffuse,
		Specular,
		Normal,
		Emissive
	};

	class Texture
	{
	public:
		GLuint ID;

		TextureType type;

		GLuint unit;

		Texture(const char* p_image, TextureType p_type, GLuint p_slot);

		void Bind();

		void Unbind();

		void Delete();
	};
}
#endif

