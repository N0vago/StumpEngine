#ifndef ST_MODEL_H
#define ST_MODEL_H

#include <json/json.h>
#include "STMesh.h"

using json = nlohmann::json;

class STModel
{
public:

	STModel(const char* file);

	void Draw(STShader& shader, STCamera& camera);

private:
	// Variables for easy access
	const char* file;
	std::vector<unsigned char> data;
	json JSON;
	
	// All the meshes and transformations
	std::vector<STMesh> meshes;
	std::vector<glm::vec3> translationsMeshes;
	std::vector<glm::quat> rotationsMeshes;
	std::vector<glm::vec3> scalesMeshes;
	std::vector<glm::mat4> matricesMeshes;

	// Prevents textures from being loaded twice
	std::vector<std::string> loadedTexName;
	std::vector<STTexture> loadedTex;

	// Loads a single mesh by its index
	void LoadMesh(unsigned int indMesh);

	// Traverses a node recursively, so it essentially traverses all connected nodes
	void TraverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

	// Gets the binary data from a file
	std::vector<unsigned char> GetData();
	// Interprets the binary data into floats, indices, and textures
	std::vector<float> GetFloats(json accessor);
	std::vector<GLuint> GetIndices(json accessor);
	std::vector<STTexture> GetTextures();

	// Assembles all the floats into vertices
	std::vector<STVertex> AssembleVertices
	(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texUVs
	);

	// Helps with the assembly from above by grouping floats
	std::vector<glm::vec2> GroupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> GroupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> GroupFloatsVec4(std::vector<float> floatVec);


};
#endif

