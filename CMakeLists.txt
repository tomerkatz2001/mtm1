cmake_minimum_required(VERSION 3.0.0)

project(map VERSION 1.0.0 LANGUAGES C)

set(MTM_FLAGES_DEBUG "-std=c99 --pedantic-errors -Wall ")
set(MTM_FLAGES_RELEASE "${MTM_FLAGES_DEBUG} -DNDEBUG")

set(CMAKE_C_FLAGS ${MTM_FLAGES_DEBUG})

add_executable(my_map map.c map.h test_utilities.h map_example_test.c)

