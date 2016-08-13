cmake_minimum_required(VERSION 2.6)
project(bacteria-core)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fPIC -ftest-coverage -fprofile-arcs") # enabling coverage

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

include_directories(unit_tests)
include_directories(integration_tests)
include_directories(src)
include_directories(src/util)
include_directories(src/model)
include_directories(src/interpreter)
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

file (GLOB integration_tests_sources
    "integration_tests/*.cpp"
)

file(GLOB unit_tests_sources
    "unit_tests/*.cpp"
)

file(GLOB lib_sources
    "src/*.cpp"
    "src/model/*.cpp"
    "src/util/*.cpp"
    "src/interpreter/*.cpp"
)

add_library(bacteria-core STATIC ${lib_sources})
TARGET_LINK_LIBRARIES(bacteria-core ${QT_LIBRARIES})

enable_testing()

add_executable(unit_test ${unit_tests_sources})
add_test(unit_test unit_test --log_level=warning)
TARGET_LINK_LIBRARIES(
    unit_test
    ${QT_LIBRARIES}
    bacteria-core
)

add_executable(integration_test ${integration_tests_sources})
TARGET_LINK_LIBRARIES(
    integration_test
    ${QT_LIBRARIES}
    yaml-cpp
    bacteria-core
)
