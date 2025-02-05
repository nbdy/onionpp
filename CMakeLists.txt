cmake_minimum_required(VERSION 3.15)
project(onionpp LANGUAGES CXX VERSION 1.0.1)

option(TORPP "Build tor++" ON)
option(SO_TEST "Build .so load test" ON)
option(PYTHON_BINDINGS "Build python bindings" ON)
option(TESTS "Build tests" ON)

include(cmake/SetVariables.cmake)
include(cmake/SetToolchain.cmake)
include(cmake/Build.cmake)
include(cmake/Tests.cmake)

unset(TORPP CACHE)
unset(SO_TEST CACHE)
unset(PYTHON_BINDINGS CACHE)
unset(TESTS CACHE)
