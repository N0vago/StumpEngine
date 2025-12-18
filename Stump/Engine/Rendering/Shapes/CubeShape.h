#ifndef ST_CUBE_SHAPE_H
#define ST_CUBE_SHAPE_H
#include "Rendering/Shapes/Shape.h"
using namespace Math;
namespace Rendering {
	class CubeShape : public Shape
	{
		float size;

	public:
		CubeShape(float p_size);

		~CubeShape();
	private:
		virtual void GenerateVertices() override;
		virtual void GenerateIndices() override;
	};
}
#endif // ST_CUBE_SHAPE_H