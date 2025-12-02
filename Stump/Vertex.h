#ifndef ST_VERTEX_H
#define ST_VERTEX_H

#include "Vector2.h"
#include "Vector3.h"

struct Vertex {
	Vector3 position;
	Vector3 normal;
	Vector3 color;
	Vector2 texCoord;
};

#endif