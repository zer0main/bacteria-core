enable_testing()

include_directories(../util/ ../model/ ../interpreter/ ..)

file(GLOB sources "../*.cpp" "../*/*.cpp")

add_executable(bacteria_test ${sources})

add_test(bacteria_test bacteria_test --log_level=warning)
