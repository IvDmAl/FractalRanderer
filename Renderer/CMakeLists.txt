add_library(Renderer
"Include/Logs.hpp"
"Include/ThreadingFunctionCall.hpp"
"Include/TimeStatistic.hpp"
"Include/Renderer.hpp"
"Include/Inputs.hpp"
"Include/WindowAndInputDataConnection.hpp"
"Include/includes.hpp"

"Source/Logs.cpp"
"Source/ThreadingFunctionCall.cpp"
"Source/TimeStatistic.cpp"
"Source/Renderer.cpp"
"Source/Inputs.cpp"
"Source/WindowAndInputDataConnection.cpp"
)

set_target_properties(Renderer PROPERTIES LINKER_LANGUAGE CXX)
set_property(TARGET Renderer PROPERTY CXX_STANDARD 20)

target_link_libraries(Renderer
	Fractals
	ColoringSchemes
	Eigen3::Eigen
	glfw
	GLEW::GLEW
)

target_include_directories(Renderer PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
  $<INSTALL_INTERFACE:include>
)

install(TARGETS Renderer)