add_library(ColoringSchemes
"Include/ColoringInterface.hpp"
"Source/ColoringInterface.cpp"
"Include/Classic.hpp"
"Source/Classic.cpp"
)

set_target_properties(ColoringSchemes PROPERTIES LINKER_LANGUAGE CXX)
set_property(TARGET ColoringSchemes PROPERTY CXX_STANDARD 20)

target_link_libraries(ColoringSchemes
	Eigen3::Eigen
)

target_include_directories(ColoringSchemes PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
  $<INSTALL_INTERFACE:include>
)

install(TARGETS ColoringSchemes)
