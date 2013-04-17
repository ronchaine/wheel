find_package(GTest REQUIRED)

set(CMAKE_CXX_FLAGS "--std=c++11 -fno-exceptions -O3 -Wall")

set(PROJECT_TEST_NAME "${PROJECT_NAME}_test_test")
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(${PROJECT_TEST_NAME} "test_vectors.cpp")
target_link_libraries(${PROJECT_TEST_NAME} ${PROJECT_NAME_STR} ${GTEST_BOTH_LIBRARIES} pthread)

add_test(${PROJECT_TEST_NAME} ${PROJECT_TEST_NAME})
