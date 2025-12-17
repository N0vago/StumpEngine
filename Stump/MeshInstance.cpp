#include "MeshInstance.h"
#include "PlaneShape.h"
MeshInstance::MeshInstance(const ObjectInfo& r_objectInfo) : SceneNode(r_objectInfo)
{
	renderObject = std::make_unique<RenderObject>();
	Texture textures[] =
	{
		Texture("planks.png", TextureType::Diffuse, 0),
		Texture("planksSpec.png", TextureType::Specular, 1)
	};
	std::vector<Texture> texs(textures, textures + sizeof(textures) / sizeof(Texture));

	Vector3 lightColor = Vector3(1.0f, 1.0f, 1.0f);
	Vector3 lightPos = Vector3(0.0f, 5.0f, 0.0f);

	RenderObject plane;
	plane.material = std::make_shared<Material>(Shader("default.vert", "default.frag"));
	plane.material->SetTexture(&textures[0]);
	plane.material->SetTexture(&textures[1]);
	plane.material->SetFloat3("lightPos", lightPos.x, lightPos.y, lightPos.z);
	plane.material->SetFloat4("lightColor", lightColor.x, lightColor.y, lightColor.z, 1);
	plane.mesh = std::make_shared<PlaneShape>(10.0f, 10.0f);
	plane.modelMatrix = Matrix3x4();
	plane.modelMatrix.Translate(Vector3(0.0f, 1.0f, 0.0f));
	plane.modelMatrix.Rotate(Vector3(0.0f, Math::ToRadians(90.0f), 0.0f));
	plane.modelMatrix.Scale(Vector3(1.f, 0.0f, 1.0f));

}

MeshInstance::MeshInstance(const ObjectInfo& r_objectInfo, std::shared_ptr<RenderObject> p_renderObject) : SceneNode(r_objectInfo), renderObject(std::move(p_renderObject))
{
}

void MeshInstance::OnAwake()
{
	RenderManager::Get().AddToRender(renderObject.get());
}

void MeshInstance::Update(float p_deltaTime)
{
}

void MeshInstance::OnSleep()
{
	RenderManager::Get().RemoveFromRender(renderObject.get());
}
