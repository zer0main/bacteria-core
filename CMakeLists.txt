cmake_minimum_required(VERSION 2.6)
project(bacteria-core)

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

subdirs(src/test)

include_directories(src)
include_directories(src/util)
include_directories(src/model)
include_directories(src/interpreter)
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

file(GLOB sources "src/*/*.cpp" "src/*.cpp")

add_executable(bacteria-core ${sources})
TARGET_LINK_LIBRARIES(bacteria-core ${QT_LIBRARIES})
