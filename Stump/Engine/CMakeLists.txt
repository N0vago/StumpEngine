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
    # ---------------- Debug -------------
    Debug/Logger.cpp
    # ---------------- ThirdParty ----------------
    ThirdParty/imgui/imgui.cpp
    ThirdParty/imgui/imgui_draw.cpp
    ThirdParty/imgui/imgui_tables.cpp
    ThirdParty/imgui/imgui_widgets.cpp
    ThirdParty/imgui/imgui_demo.cpp
    ThirdParty/imgui/backends/imgui_impl_glfw.cpp
    ThirdParty/imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
	${STUMP_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui/backend
)
target_link_libraries(Engine PUBLIC
    glfw
    OpenGL::GL
    OpenAL::OpenAL
    glad
)
target_compile_definitions(Engine PUBLIC
    STUMP_ENGINE
    IMGUI_IMPL_GLFW_USE_OPENGL3
    IMGUI_IMPL_OPENGL_LOADER_GLAD
)
