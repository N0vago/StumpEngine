#ifndef ST_OBJECT_H
#define ST_OBJECT_H

#include "Matrix3x4.h";
#include "Mesh.h"

#include <string>


struct ObjectInfo {
	int long id;

	std::string name = "Object";

	Matrix3x4 transform = Matrix3x4();
};

class Object
{
	ObjectInfo info;

public:

	Object(ObjectInfo p_info) : info(p_info) {}

	ObjectInfo& GetInfo() { return info; }

	virtual void AddToScene() = 0;


};
#endif // OBJECT_H

