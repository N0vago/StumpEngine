#ifndef ST_OBJECT_H
#define ST_OBJECT_H

#include "Rendering/Mesh.h"
#include "Math/Matrix3x4.h";

#include <string>

namespace Scene {
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
}
#endif // OBJECT_H

