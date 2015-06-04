# Use musl static linked version or find
find_package(PhysFS REQUIRED)
#set(PHYSFS_LIBRARY ${WHEEL_SOURCE_DIR}/lib/libphysfs.a)

include_directories(${WHEEL_SOURCE_DIR}/src
                    ${WHEEL_SOURCE_DIR}/include)

#set(COMMON_HEADERS ${WHEEL_SOURCE_DIR}/include/wheel_core.h utf8.h)
set(COMMON_SOURCES core.cpp debug.cpp module.cpp string.cpp resource.cpp
                   utility.cpp library.cpp atlas.cpp event.cpp)

set(IMAGE_SOURCES image/image.cpp image/png.cpp)
set(IMAGE_HEADERS ${WHEEL_SOURCE_DIR}/include/wheel_image.h)

#set(SOUND_SOURCES sound/sound.cpp sound/library.cpp sound/wav.cpp)
#set(SOUND_HEADERS ${WHEEL_SOURCE_DIR}/include/wheel_sound.h)

set(WHEEL_SOURCES ${COMMON_SOURCES})
set(WHEEL_HEADERS ${COMMON_HEADERS})

set(WHEEL_LIBRARIES_UNIX dl)

# Remove if not required anymore
if (APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

# glibc bug may cause this to be required
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__extern_always_inline=inline")

if (WIN32)
set(WHEEL_LIBRARIES ${PHYSFS_LIBRARY})
else()
set(WHEEL_LIBRARIES ${PHYSFS_LIBRARY} ${WHEEL_LIBRARIES_UNIX})
endif()

# Image library
if (WHEEL_INCLUDE_IMAGELIBRARY)
  set(WHEEL_SOURCES ${WHEEL_SOURCES} ${IMAGE_SOURCES})
  set(WHEEL_HEADERS ${WHEEL_HEADERS} ${IMAGE_HEADERS})
endif()

# Sound library
if (WHEEL_INCLUDE_SOUNDLIBRARY)
  set(WHEEL_SOURCES ${WHEEL_SOURCES} ${SOUND_SOURCES})
  set(WHEEL_HEADERS ${WHEEL_HEADERS} ${SOUND_HEADERS})
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -fno-exceptions -fno-rtti -O2 -fPIC -g")

add_library(wheel_core SHARED ${WHEEL_SOURCES} ${WHEEL_HEADERS})
set_target_properties(wheel_core PROPERTIES OUTPUT_NAME "${WHEEL_LIB_NAME}")
set_target_properties(wheel_core PROPERTIES VERSION ${WHEEL_VERSION})
target_link_libraries(wheel_core ${WHEEL_LIBRARIES})

if (WHEEL_BUILD_STATIC)
   add_library(wheel_core_static STATIC ${WHEEL_SOURCES} ${WHEEL_HEADERS})
   set_target_properties(wheel_core_static PROPERTIES OUTPUT_NAME "${WHEEL_LIB_NAME}_static")
   set_target_properties(wheel_core_static PROPERTIES VERSION ${WHEEL_VERSION})
   target_link_libraries(wheel_core_static ${WHEEL_LIBRARIES})
endif()

# Install targets
install (TARGETS wheel_core DESTINATION lib)

if (WHEEL_BUILD_STATIC)
   install (TARGETS wheel_core_static DESTINATION lib)
endif()

file(GLOB files ${CMAKE_CURRENT_SOURCE_DIR}/../include/*.h)
foreach(filename ${files})
   install (FILES ${filename} DESTINATION include/wheel)
endforeach()
