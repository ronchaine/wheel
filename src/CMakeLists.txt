include_directories(${WHEEL_SOURCE_DIR}/src
                    ${WHEEL_SOURCE_DIR}/include)

set(COMMON_HEADERS ${WHEEL_SOURCE_DIR}/include/wheel_core.h utf8.h)
set(COMMON_SOURCES core.cpp module.cpp string.cpp resourcemanager.cpp
                   utility.cpp)

set(WHEEL_SOURCES ${COMMON_SOURCES})
set(WHEEL_HEADERS ${COMMON_HEADERS})

set(CMAKE_CXX_FLAGS "--std=c++11 -fno-exceptions -fno-rtti -O3")

add_library(wheel_core SHARED ${WHEEL_SOURCES} ${WHEEL_HEADERS})
add_library(wheel_core_static STATIC ${WHEEL_SOURCES} ${WHEEL_HEADERS})

set_target_properties(wheel_core PROPERTIES OUTPUT_NAME "${WHEEL_LIB_NAME}")
set_target_properties(wheel_core PROPERTIES VERSION ${WHEEL_VERSION})

set_target_properties(wheel_core_static PROPERTIES OUTPUT_NAME "${WHEEL_LIB_NAME}_static")
set_target_properties(wheel_core_static PROPERTIES VERSION ${WHEEL_VERSION})
