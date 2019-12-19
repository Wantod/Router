
cmake_minimum_required(VERSION 3.12)
project(router)

set(CLIENT_NAME "client")
set(TEST_NAME "test_client")

message("Generation avec la configuration ${CMAKE_CONFIGURATION_TYPES} ${CMAKE_BUILD_TYPE}")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/)
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Compilation flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# project
file(GLOB_RECURSE CLIENT_SRC src/router/*.cpp src/router/*.hpp)
add_executable(${CLIENT_NAME} "src/main.cpp" ${CLIENT_SRC})

# test
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE lib/Catch2/single_include/)

file(GLOB_RECURSE TEST_SRC src/test/*.cpp src/test/*.hpp)
add_executable(${TEST_NAME} "src/main_test.cpp" ${TEST_SRC}  ${CLIENT_SRC})
target_link_libraries(${TEST_NAME} Catch)

enable_testing()
add_test(test ${TEST_NAME})

if (CMAKE_COMPILER_IS_GNUCC)
    target_compile_options(${CLIENT_NAME} PRIVATE "-Wall")
    target_compile_options(${TEST_NAME} PRIVATE "-Wall")
endif()
if ( MSVC )
    target_compile_options(${CLIENT_NAME} PRIVATE "/W4")
    target_compile_options(${TEST_NAME} PRIVATE "/W4")
endif()