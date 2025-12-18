#ifndef ST_SHAPE_H
#define ST_SHAPE_H
#include "Rendering/Mesh.h"
namespace Rendering {
	class Shape : public Mesh
	{
	protected:
		Shape() {}

		virtual void GenerateVertices() = 0;
		virtual void GenerateIndices() = 0;
	public:
		~Shape() = default;

	};
}
#endif // ST_SHAPE_H

