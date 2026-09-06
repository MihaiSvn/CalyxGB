cmake_minimum_required(VERSION 3.10)

#name of project and language
project(CalyxGBHardware C)

#where the .h files are
include_directories(include)

#executable and the source file from where it's compiled
add_executable(test_buttons src/test_buttons.c)

# adding lgpio library
target_link_libraries(test_buttons lgpio)