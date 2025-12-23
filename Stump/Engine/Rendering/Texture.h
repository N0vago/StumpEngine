#ifndef ST_TEXTURE_H
#define ST_TEXTURE_H

#include <type_traits>
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
		uint32_t ID;

		TextureType type;

		uint32_t unit;

		Texture(const char* p_image, TextureType p_type, uint32_t p_slot);

		void Bind();

		void Unbind();

		void Delete();
	};
}
#endif

