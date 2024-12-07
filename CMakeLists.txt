cmake_minimum_required(VERSION 3.15...3.26)
project(onionpp LANGUAGES CXX VERSION 1.0.0)

option(WINDOWS "Build for windows" OFF)
option(x86_64 "Build for x86_64" OFF)
option(EXAMPLE "Build example" OFF)
option(SO_TEST "Build .so load test" OFF)

set(OPTION_LIST WINDOWS x86_64 EXAMPLE SO_TEST)

include(cmake/SetVariables.cmake)
include(cmake/SetToolchain.cmake)
include(cmake/BuildLibrary.cmake)
include(cmake/BuildExample.cmake)

# unset options
foreach(option ${OPTION_LIST})
    unset(${option} CACHE)
endforeach()
