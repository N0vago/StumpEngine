cmake_minimum_required(VERSION 3.20)

project(StumpEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------- OpenGL ----------------
find_package(OpenGL REQUIRED)

# ---------------- OpenAL ----------------
find_package(OpenAL CONFIG REQUIRED)

# ----------------- sndlib ----------------------

find_package(SndFile CONFIG REQUIRED)
# ---------------- Paths ----------------
set(STUMP_LIB_DIR     ${CMAKE_SOURCE_DIR}/Libraries)
set(STUMP_INCLUDE_DIR ${STUMP_LIB_DIR}/include)
set(STUMP_LIB_BIN     ${STUMP_LIB_DIR}/lib)

# ---------------- glad ----------------
add_library(glad STATIC glad.c)
target_include_directories(glad PUBLIC
    ${STUMP_INCLUDE_DIR}
)

# ---------------- stb ----------------
add_library(stb STATIC stb.cpp)
target_include_directories(stb PUBLIC
    ${STUMP_INCLUDE_DIR}
)

# ---------------- GLFW (prebuilt) ----------------
add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION ${STUMP_LIB_BIN}/glfw3.lib
)

target_include_directories(glfw INTERFACE
    ${STUMP_INCLUDE_DIR}
)

# ---------------- Engine ----------------
add_subdirectory(Engine)

# ---------------- Executable ----------------
add_executable(Stump
    Engine/Main/main.cpp
)

target_link_libraries(Stump PRIVATE
    Engine
    glfw
    OpenGL::GL
    OpenAL::OpenAL
    SndFile::sndfile
    glad
    stb
)
target_include_directories(Stump PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Engine
)
add_custom_target(CopyAssets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Engine/Assets
        $<TARGET_FILE_DIR:Stump>/Assets
)

add_dependencies(Stump CopyAssets)