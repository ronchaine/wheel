project(WHEEL CXX)

cmake_minimum_required(VERSION 2.8)

set(WHEEL_VERSION_MAJOR "0")
set(WHEEL_VERSION_MINOR "1")
set(WHEEL_VERSION_PATCH "0")
set(WHEEL_VERSION_EXTRA "")

set(WHEEL_VERSION "${WHEEL_VERSION_MAJOR}.${WHEEL_VERSION_MINOR}.${WHEEL_VERSION_PATCH}${WHEEL_VERSION_EXTRA}")

set(WHEEL_LIB_NAME wheel)

option (WHEEL_BUILD_MODULES "Build the basic modules" OFF)

#--------------------------------------------------------------
# Add subdirectories
#--------------------------------------------------------------

add_subdirectory(src)

if (WHEEL_BUILD_MODULES)
  add_subdirectory(src/modules)
endif()
