enable_testing()

include_directories(../src/util/ ../src/model/ ../src/interpreter/ ../src .)

file(GLOB sources "../src/*/*.cpp" "*.cpp")

add_executable(bacteria_test ${sources})

add_test(bacteria_test bacteria_test --log_level=warning)
