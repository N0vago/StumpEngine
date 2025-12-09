#ifndef ST_OBJECT_H
#define ST_OBJECT_H

#include "Matrix3x4.h";
#include "Mesh.h"

#include <string>


struct ObjectInfo {
	int long id;

	std::string name = "Object";
};

class Object
{
protected:
	ObjectInfo info;

public:

	Object(const ObjectInfo& p_info) : info(p_info) {}

	ObjectInfo& GetInfo() { return info; }

};
#endif // OBJECT_H

