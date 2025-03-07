add_library(Fractals
"Include/FractalInterface.hpp"
"Source/FractalInterface.cpp"
"Include/Mandelbrot.hpp"
"Source/Mandelbrot.cpp"
)

set_target_properties(Fractals PROPERTIES LINKER_LANGUAGE CXX)
set_property(TARGET Fractals PROPERTY CXX_STANDARD 20)

target_include_directories(Fractals PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
  $<INSTALL_INTERFACE:include>
)

install(TARGETS Fractals)
