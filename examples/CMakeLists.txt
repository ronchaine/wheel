include_directories(${WHEEL_SOURCE_DIR}/src
                    ${WHEEL_SOURCE_DIR}/include)

link_directories(${WHEEL_BINARY_DIR}/src)

set(CMAKE_CXX_FLAGS "--std=c++11 -fno-exceptions -fno-rtti -O3 -Wall")

add_executable(stringhash stringhash.cpp)
target_link_libraries(stringhash wheel)

add_executable(modinfo modinfo.cpp)
target_link_libraries(modinfo wheel)

add_executable(triangle triangle.cpp)
target_link_libraries(triangle wheel GL)

add_executable(modenum enum-plugins.cpp)
target_link_libraries(modenum wheel)
