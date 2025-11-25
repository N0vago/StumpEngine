#include "Model.h"
/*
* TODO: Remove glm dependency
* Separate logic of loading and parsing of a model from rendering logic
* I guess that I should do a struct that holds model data and another class that handles rendering
* 
* NOTE: Previous operations on camera matrix is not representing transformation of simple model
*/

void Model::Draw(Shader& shader, Camera& camera)
{
	// Go over all meshes and draw each one
	for (unsigned int i = 0; i < modelData.meshes.size(); i++)
	{
		modelData.meshes[i].first.Draw(shader, camera, modelData.meshes[i].second);
	}
}

