cmake_minimum_required(VERSION 3.15...3.26)
project(onionpp LANGUAGES CXX VERSION 1.0.0)

option(WINDOWS "Build for windows" OFF)
option(x86_64 "Build for x86_64" OFF)
option(TORPP "Build tor++" ON)
option(SO_TEST "Build .so load test" OFF)

set(OPTION_LIST WINDOWS x86_64 TORPP SO_TEST)

include(cmake/SetVariables.cmake)
include(cmake/SetToolchain.cmake)
include(cmake/Build.cmake)

# unset options
foreach(option ${OPTION_LIST})
    unset(${option} CACHE)
endforeach()
