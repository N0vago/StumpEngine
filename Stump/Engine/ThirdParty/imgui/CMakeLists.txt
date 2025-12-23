add_library(imgui STATIC
    imgui.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui_demo.cpp
    backends/imgui_impl_glfw.cpp
    backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/backends
)

target_compile_definitions(imgui PRIVATE
    IMGUI_IMPL_GLFW_USE_OPENGL3
    IMGUI_IMPL_OPENGL_LOADER_GLAD
)

target_link_libraries(imgui PRIVATE
    glfw
    glad
    OpenGL::GL
)
