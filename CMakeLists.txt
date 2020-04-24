cmake_minimum_required(VERSION 3.0.0)

project(map VERSION 0.1.0 LANGUAGES C)

set (MTM_FLAGES_DEBUG "-std=c99 --pedantic-errors -Wall -Werror")
set (MTM_FALGES_RELEASE "{MTM_FLAGES_DEBUG} -DNDEBUG")

set(CMAKE_C_FLAGES ${MTM_FLAGES_DEBUG})

add_executable(my_executable map.c)