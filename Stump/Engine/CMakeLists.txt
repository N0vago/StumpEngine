add_library(Engine)

target_sources(Engine PRIVATE

    # ---------------- Core ----------------
    Core/Application.cpp
    Core/InputManager.cpp
    Core/Layer.cpp
    Core/Window.cpp
    Core/Layers/EngineModeLayer.cpp

    # ---------------- Math ----------------
    Math/AABB.cpp
    Math/MathF.cpp
    Math/Matrix3x3.cpp
    Math/Matrix3x4.cpp
    Math/Matrix4x4.cpp
    Math/Plane.cpp
    Math/Quaternion.cpp
    Math/Vector2.cpp
    Math/Vector3.cpp
    Math/Vector4.cpp

    # ---------------- Physic ----------------
    Physic/Collider.cpp
    Physic/PhysicWorld.cpp
    Physic/RigidBody.cpp

    # ---------------- Rendering ----------------
    Rendering/Buffers/ElementsBufferObj.cpp
    Rendering/Buffers/VertexBufferObj.cpp
    Rendering/Buffers/VertexArrayObj.cpp
    Rendering/Shapes/Shape.cpp
    Rendering/Shapes/CubeShape.cpp
    Rendering/Shapes/SphereShape.cpp
    Rendering/Shapes/PlaneShape.cpp
    Rendering/Camera.cpp
    Rendering/Material.cpp
    Rendering/Mesh.cpp
    Rendering/RenderManager.cpp
    Rendering/Shader.cpp
    Rendering/Texture.cpp

    # ---------------- Scene ----------------
    Scene/Scene.cpp

    Scene/Object.cpp
    Scene/SceneNode.cpp
    Scene/GameObject.cpp

    # ---------------- Utility ----------------
    Utility/ModelData.cpp

    # ---------------- Audio ------------------
    Audio/AudioDevice.cpp
    Audio/AudioBuffer.cpp
    Audio/AudioSource.cpp
    Audio/AudioListener.cpp
    # ---------------- Components -------------
    Components/RigidBodyComponent.cpp
    Components/ColliderComponent.cpp
    Components/CameraComponent.cpp
    Components/MeshRendererComponent.cpp
    Components/AudioSourceComponent.cpp
    )
target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
	${STUMP_INCLUDE_DIR}
)
target_link_libraries(Engine PUBLIC
    glfw
    OpenGL::GL
    OpenAL::OpenAL
    glad
)
target_compile_definitions(Engine PUBLIC STUMP_ENGINE)
