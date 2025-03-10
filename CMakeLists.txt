cmake_minimum_required (VERSION 3.8)

project (FractalRenderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v")

find_package(Eigen3 REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glew REQUIRED)


add_subdirectory(ColoringSchemes)
add_subdirectory(Fractals)
add_subdirectory(Renderer)

add_executable(renderer_executable renderer.cpp)
target_link_libraries(renderer_executable
ColoringSchemes
Fractals
Renderer
)
if (MSVC)
set_target_properties(${BUILD_TARGET} PROPERTIES LINK_FLAGS "/PROFILE")
endif()