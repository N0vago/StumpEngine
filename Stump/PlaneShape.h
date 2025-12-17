#ifndef ST_PLANE_SHAPE_H
#define ST_PLANE_SHAPE_H
#include "Shape.h"
class Shader;
class PlaneShape : public Shape
{
	float width;
	float height;
public:
	PlaneShape(float p_width, float p_height);
private:
	virtual void GenerateVertices() override;
	virtual void GenerateIndices() override;
};

#endif // ST_PLANE_SHAPE_H

