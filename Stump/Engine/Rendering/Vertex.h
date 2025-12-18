#ifndef ST_VERTEX_H
#define ST_VERTEX_H

#include "Math/Vector2.h"
#include "Math/Vector3.h"
namespace Rendering {
	struct Vertex {
		Math::Vector3 position;
		Math::Vector3 normal;
		Math::Vector3 color;
		Math::Vector2 texCoord;
	};
}
#endif