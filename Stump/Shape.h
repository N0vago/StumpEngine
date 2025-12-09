#ifndef ST_SHAPE_H
#define ST_SHAPE_H
#include "Mesh.h"
class Shape : public Mesh
{
protected:
	Shape(Shader& shader) : Mesh(shader) {}

	virtual void GenerateVertices() = 0;
	virtual void GenerateIndices() = 0;
public:
	~Shape() = default;

};
#endif // ST_SHAPE_H

