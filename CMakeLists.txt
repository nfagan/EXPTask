cmake_minimum_required(VERSION 2.8)
project(EXPTask)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(EXPTask_LIBRARY_DIR, ${CMAKE_SOURCE_DIR}/lib)

get_filename_component(EXP_ROOT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY)

list(APPEND CMAKE_MODULE_PATH ${EXP_ROOT_DIR}/EXPUtil)
list(APPEND CMAKE_MODULE_PATH ${EXP_ROOT_DIR}/EXPGL)

find_package(EXPGL REQUIRED)
find_package(EXPUtil REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include/)
include_directories(${EXPUtil_INCLUDE_DIR})
include_directories(${EXPGL_INCLUDE_DIR})

link_directories(${EXPGL_LIBRARY_DIR})
link_directories(${EXPUtil_LIBRARY_DIR})
link_directories(${EXPTask_LIBRARY_DIR})

file(GLOB_RECURSE SOURCES "src/EXPTask/*.c" "src/EXPTask/*.cpp" "include/EXPTask/*.hpp" "include/EXPTask/*.h")

add_library(EXPTask STATIC ${SOURCES})
target_link_libraries(EXPTask ${EXPGL_LIBRARIES} ${EXPUtil_LIBRARIES})

# Test

file(GLOB_RECURSE TESTSOURCES "test/main.cpp" "test/include/*.hpp")
include_directories(${CMAKE_SOURCE_DIR}/test/include)
add_executable(EXPTask_test ${TESTSOURCES})
target_link_libraries(EXPTask_test ${EXPGL_LIBRARIES} ${EXPUtil_LIBRARIES} EXPTask)

install(TARGETS EXPTask DESTINATION ${CMAKE_SOURCE_DIR}/lib)
install(TARGETS EXPTask_test DESTINATION ${CMAKE_SOURCE_DIR}/bin)