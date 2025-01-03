# start cmake with the following command:
#
# mkdir build
# cmake -Bbuild .
# cmake --build build
#
cmake_minimum_required(VERSION 3.12)

project(Tetris)

find_package(OpenGL REQUIRED)

add_executable(Tetris
    Block.cpp
    Field.cpp
    GameObject.cpp
    Matrix4x4.cpp
    Particle.cpp
    Plane.cpp
    Quaternion.cpp
    Vector3.cpp
    glad.c
    main.cpp
    stb_truetype.cpp
    uiFont.cpp
)

# add dependencies
include(cmake/CPM.cmake)

CPMAddPackage("gh:g-truc/glm#0.9.9.8")
CPMAddPackage("gh:SFML/SFML#2.5.0")

target_include_directories(Tetris
    PRIVATE
        include
)

target_link_libraries(Tetris
    PRIVATE
        ${OpenGL_LIBRARIES}
        glm
        sfml-system
        sfml-graphics
        sfml-window
        sfml-audio
)
