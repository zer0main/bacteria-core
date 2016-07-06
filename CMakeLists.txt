cmake_minimum_required(VERSION 2.6)
project(bacteria-core)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fPIC -ftest-coverage -fprofile-arcs") # enabling coverage

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

include_directories(test)
include_directories(src)
include_directories(src/util)
include_directories(src/model)
include_directories(src/interpreter)
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

file(GLOB test_sources
    "test/*.cpp"
    "src/model/*.cpp"
    "src/util/*.cpp"
    "src/interpreter/*.cpp"
)

file(GLOB lib_sources
    "src/model/*.cpp"
    "src/util/*.cpp"
    "src/interpreter/*.cpp"
)

enable_testing()

add_executable(bacteria_test ${test_sources})
add_test(bacteria_test bacteria_test --log_level=warning)

add_library(bacteria-core SHARED ${lib_sources})
TARGET_LINK_LIBRARIES(bacteria-core ${QT_LIBRARIES})
