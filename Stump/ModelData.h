#ifndef ST_MODELDATA_H
#define ST_MODELDATA_H

#include <vector>
#include <json/json.h>
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Mesh.h"

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

	// Prevents textures from being loaded twice
	std::vector<std::string> loadedTexName;
	std::vector<Texture> loadedTex;

	// Loads a single mesh by its index
	Mesh LoadMesh(unsigned int indMesh);

	// Traverses a node recursively, so it essentially traverses all connected nodes
	void TraverseNode(unsigned int nextNode, Matrix3x4 matrix = Matrix3x4());

	// Gets the binary data from a file
	std::vector<unsigned char> GetData();
	// Interprets the binary data into floats, indices, and textures
	std::vector<float> GetFloats(json accessor);
	std::vector<GLuint> GetIndices(json accessor);
	std::vector<Texture> GetTextures();

	// Assembles all the floats into vertices
	std::vector<Vertex> AssembleVertices
	(
		std::vector<Vector3> positions,
		std::vector<Vector3> normals,
		std::vector<Vector2> texUVs
	);

	// Helps with the assembly from above by grouping floats
	std::vector<Vector2> GroupFloatsVec2(std::vector<float> floatVec);
	std::vector<Vector3> GroupFloatsVec3(std::vector<float> floatVec);
	std::vector<Vector4> GroupFloatsVec4(std::vector<float> floatVec);
};
#endif // ST_MODELDATA_H

