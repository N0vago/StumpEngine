#ifndef ST_SPHERE_SHAPE_H
#define ST_SPHERE_SHAPE_H
#include "Rendering/Shapes/Shape.h"
using namespace Math;
namespace Rendering {
	class SphereShape : public Shape
	{
		float radius;
		int sectorCount;
		int stackCount;
	public:
		SphereShape(float p_radius, int p_sectorCount, int p_stackCount);

	private:
		virtual void GenerateVertices() override;
		virtual void GenerateIndices() override;
	};
}
#endif // ST_SPHERE_SHAPE_H