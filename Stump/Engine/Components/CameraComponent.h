#ifndef ST_CAMERA_COMPONENT_H
#define ST_CAMERA_COMPONENT_H
#include "Components/Component.h"
#include "Rendering/Camera.h"

#include <memory>
namespace Components {
	class CameraComponent : public Component {
		//TODO: check pos updating, add SetLookAt()
		std::shared_ptr<Rendering::Camera> camera;
	public:
		float FOVdeg;
		float nearPlane;
		float farPlane;

		CameraComponent();
		~CameraComponent();

		void OnEnable() override;
		void OnDisable() override;

		void Update(float p_deltaTime) override;

	};
}
#endif //ST_CAMERA_COMPONENT_H