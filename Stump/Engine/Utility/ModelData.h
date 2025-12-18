#ifndef ST_MODELDATA_H
#define ST_MODELDATA_H

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Quaternion.h"
#include "Math/Matrix3x4.h"
#include "Rendering/Mesh.h"
#include "Rendering/Texture.h"
#include <vector>
#include <json/json.h>

using namespace Math;
using namespace Rendering;
using json = nlohmann::json;


class ModelData
{
public:
	ModelData(const char* filePath);

	std::vector<std::pair<Mesh, Matrix3x4>> meshes;
private:
	const char* file;
	std::vector<unsigned char> data;
	json JSON;

	std::vector<std::string> loadedTexName;
	std::vector<Texture> loadedTex;

	Mesh LoadMesh(unsigned int indMesh);

	void TraverseNode(unsigned int nextNode, Matrix3x4 matrix = Matrix3x4());

	std::vector<unsigned char> GetData();
	
	std::vector<float> GetFloats(json accessor);
	std::vector<GLuint> GetIndices(json accessor);
	std::vector<Texture> GetTextures();

	std::vector<Vertex> AssembleVertices
	(
		std::vector<Math::Vector3> positions,
		std::vector<Math::Vector3> normals,
		std::vector<Vector2> texUVs
	);

	std::vector<Vector2> GroupFloatsVec2(std::vector<float> floatVec);
	std::vector<Math::Vector3> GroupFloatsVec3(std::vector<float> floatVec);
	std::vector<Vector4> GroupFloatsVec4(std::vector<float> floatVec);
};
#endif // ST_MODELDATA_H

