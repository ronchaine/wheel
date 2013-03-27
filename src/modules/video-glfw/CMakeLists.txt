find_package(GLFW3 REQUIRED)

include_directories(${WHEEL_SOURCE_DIR}/src
                    ${WHEEL_SOURCE_DIR}/include
                    /usr/local/include
                    ${GLFW_INCLUDE_DIR})

#link_directories()

set(MODULE_VERSION 0.1)

set(MODULE_HEADERS ${WHEEL_SOURCE_DIR}/include/wheel_core_module.h
                    video-glfw.h)
set(MODULE_SOURCES video-glfw.cpp)

set(CMAKE_CXX_FLAGS "--std=c++11 -fno-exceptions -fno-rtti -O3")

add_library(wheel_video_glfw SHARED ${MODULE_SOURCES} ${MODULE_HEADERS})

TARGET_LINK_LIBRARIES(wheel_video_glfw ${GLFW_LIBRARY})

set_target_properties(wheel_video_glfw PROPERTIES OUTPUT_NAME "wcl_video_glfw")
set_target_properties(wheel_video_glfw PROPERTIES VERSION ${MODULE_VERSION})
