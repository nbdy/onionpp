cmake_minimum_required(VERSION 3.15)
project(onionpp LANGUAGES CXX VERSION 1.0.1)

option(TORPP "Build tor++" ON)
option(SO_TEST "Build .so load test" ON)
option(PYTHON_BINDINGS "Build python bindings" ON)

include(cmake/SetVariables.cmake)
include(cmake/SetToolchain.cmake)
include(cmake/Build.cmake)

unset(TORPP CACHE)
unset(SO_TEST CACHE)
