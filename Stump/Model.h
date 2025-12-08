#ifndef ST_MODEL_H
#define ST_MODEL_H

#include "ModelData.h"
#include "Camera.h"
#include "Shader.h"

class Model
{
	ModelData modelData;

public:

	Model(ModelData data) : modelData(data) {}


	void Draw(Shader& shader, Camera& camera);

};
#endif

