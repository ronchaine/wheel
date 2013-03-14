include_directories(${WHEEL_SOURCE_DIR}/src
                    ${WHEEL_SOURCE_DIR}/include)

set(MODULE_VERSION 0.1)

set(MODULE_HEADERS ${WHEEL_SOURCE_DIR}/include/wheel_core_module.h
                    sdl-opengl.h)
set(MODULE_SOURCES sdl-opengl.cpp)

set(CMAKE_CXX_FLAGS "--std=c++11 -fno-exceptions -fno-rtti -O3")

add_library(wheel_video_opengl_sdl2 SHARED ${MODULE_SOURCES} ${MODULE_HEADERS})

set_target_properties(wheel_video_opengl_sdl2 PROPERTIES OUTPUT_NAME "wcl_video_ogl_sdl2")
set_target_properties(wheel_video_opengl_sdl2 PROPERTIES VERSION ${MODULE_VERSION})