#ifndef ST_MODEL_H
#define ST_MODEL_H

#include "ModelData.h"

class Model
{
public:

	Model(ModelData data) : modelData(data) {}

	ModelData modelData;

	void Draw(Shader& shader, Camera& camera);

};
#endif

