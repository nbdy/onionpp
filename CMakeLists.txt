cmake_minimum_required(VERSION 3.14..3.21)
# ------------
option(PYTHON_BINDINGS "Build python bindings" OFF)
# ------------
cmake_minimum_required(VERSION 3.4)
project(onionpp LANGUAGES CXX)

option(WINDOWS "Build for windows" OFF)
option(x86_64 "Build for x86_64" OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(WINDOWS_PYTHON_DIRECTORY "")
set(TOOLCHAIN_PREFIX "")

include(ExternalProject)

if(WINDOWS)
    set(CMAKE_SYSTEM_NAME Windows)
    if(x86_64)
        set(TOOLCHAIN x86_64-w64-mingw32)
        set(TOOLCHAIN_PREFIX ${TOOLCHAIN}-)
    else()
        set(TOOLCHAIN i686-w64-mingw32)
        set(TOOLCHAIN_PREFIX ${TOOLCHAIN}-)
    endif()
    set(TOOLCHAIN_LIB_PATH "/usr/${TOOLCHAIN}/lib")

    set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
    set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)

    include_directories(mingw-std-threads)
endif()

include(cmake/AddTor.cmake)

# Static library
add_library(onionpp onionpp.h)
add_dependencies(onionpp tor)
set_target_properties(onionpp PROPERTIES LINKER_LANGUAGE CXX)

# Test executable
add_executable(onionpp_test main.cpp onionpp.h)
add_dependencies(onionpp_test tor)

# Python bindings
if(PYTHON_BINDINGS)
    find_package(pybind11 CONFIG REQUIRED)
    pybind11_add_module(onionpy python_binding.cpp)
    add_dependencies(onionpy tor)
endif()

if(WINDOWS)
    target_link_libraries(onionpp_test PRIVATE -static -static-libgcc ${TOR_LIBRARIES} ws2_32 crypt32 shlwapi iphlpapi gdi32)
else()
    target_link_libraries(onionpp_test PRIVATE ${TOR_LIBRARIES} pthread m dl)
endif()

