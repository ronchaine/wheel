project(WHEEL CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)

cmake_minimum_required(VERSION 2.8)

set(WHEEL_VERSION_MAJOR "0")
set(WHEEL_VERSION_MINOR "1")
set(WHEEL_VERSION_PATCH "1")
set(WHEEL_VERSION_EXTRA "")

set(WHEEL_VERSION "${WHEEL_VERSION_MAJOR}.${WHEEL_VERSION_MINOR}.${WHEEL_VERSION_PATCH}${WHEEL_VERSION_EXTRA}")

set(WHEEL_LIB_NAME wheel)

option (WHEEL_INCLUDE_IMAGELIBRARY "Include image loading functionality" ON)
option (WHEEL_INCLUDE_SOUNDLIBRARY "Include sound loading functionality" ON)
option (WHEEL_BUILD_MODULES "Build the basic modules" OFF)
option (WHEEL_BUILD_EXAMPLES "Build the examples (needs basic modules)" OFF)
option (WHEEL_BUILD_TESTS "Build the unit tests" OFF)

#--------------------------------------------------------------
# Add subdirectories
#--------------------------------------------------------------

  enable_testing()

add_subdirectory(src)

if (WHEEL_BUILD_EXAMPLES)
   add_subdirectory(examples)
endif()
if (WHEEL_BUILD_MODULES)
  add_subdirectory(src/modules)
endif()

if (WHEEL_BUILD_TESTS)
  add_subdirectory(tests)
endif()

