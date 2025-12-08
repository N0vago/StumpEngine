#ifndef ST_SHAPE_H
#define ST_SHAPE_H

#include "Object.h"
#include "Mesh.h"

enum class ShapeType {
	Cube,
	Sphere,
	Plane,
	Pyramid,
	Cylinder
};


class Shape : public Object
{
	Mesh shapeMesh;

public:
	Shape(ObjectInfo p_info);
	virtual void AddToScene() override;

};

#endif // ST_SHAPE_H

