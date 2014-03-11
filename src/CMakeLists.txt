find_package(PhysFS REQUIRED)

include_directories(${WHEEL_SOURCE_DIR}/src
                    ${WHEEL_SOURCE_DIR}/include)

set(COMMON_HEADERS ${WHEEL_SOURCE_DIR}/include/wheel_core.h utf8.h)
set(COMMON_SOURCES core.cpp debug.cpp module.cpp string.cpp resource.cpp
                   utility.cpp)

set(IMAGE_SOURCES image/image.cpp image/png.cpp image/library.cpp image/atlas.cpp)
set(IMAGE_HEADERS ${WHEEL_SOURCE_DIR}/include/wheel_image.h)

set(SOUND_SOURCES sound/sound.cpp sound/library.cpp sound/wav.cpp)
set(SOUND_HEADERS ${WHEEL_SOURCE_DIR}/include/wheel_sound.h)

set(WHEEL_SOURCES ${COMMON_SOURCES})
set(WHEEL_HEADERS ${COMMON_HEADERS})

set(WHEEL_LIBRARIES_UNIX dl)

if (APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

set(WHEEL_LIBRARIES ${PHYSFS_LIBRARY} ${WHEEL_LIBRARIES_UNIX})

# Image library
if (WHEEL_INCLUDE_IMAGELIBRARY)
  set(WHEEL_SOURCES ${WHEEL_SOURCES} ${IMAGE_SOURCES})
  set(WHEEL_HEADERS ${WHEEL_HEADERS} ${IMAGE_HEADERS})
endif()

# Image library
if (WHEEL_INCLUDE_SOUNDLIBRARY)
  set(WHEEL_SOURCES ${WHEEL_SOURCES} ${SOUND_SOURCES})
  set(WHEEL_HEADERS ${WHEEL_HEADERS} ${SOUND_HEADERS})
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -fno-exceptions -fno-rtti -O3 -fPIC")

add_library(wheel_core SHARED ${WHEEL_SOURCES} ${WHEEL_HEADERS})
add_library(wheel_core_static STATIC ${WHEEL_SOURCES} ${WHEEL_HEADERS})

set_target_properties(wheel_core PROPERTIES OUTPUT_NAME "${WHEEL_LIB_NAME}")
set_target_properties(wheel_core PROPERTIES VERSION ${WHEEL_VERSION})

set_target_properties(wheel_core_static PROPERTIES OUTPUT_NAME "${WHEEL_LIB_NAME}_static")
set_target_properties(wheel_core_static PROPERTIES VERSION ${WHEEL_VERSION})

target_link_libraries(wheel_core ${WHEEL_LIBRARIES})
