cmake_minimum_required(VERSION 3.15..3.26)
project(onionpp LANGUAGES CXX)

option(WINDOWS "Build for windows" OFF)
option(x86_64 "Build for x86_64" OFF)
option(PYTHON "Build python module" OFF)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(WINDOWS_PYTHON_DIRECTORY "")
set(TOOLCHAIN_PREFIX "")

set(CMAKE_CXX_FLAGS "-Wall -Wextra -fstack-protector-all")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(include)

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

    message("Using GCC: ${CMAKE_C_COMPILER}")
    message("Using CXX: ${CMAKE_CXX_COMPILER}")
    if (DEFINED TOOLCHAIN)
        message("Toolchain: ${TOOLCHAIN} (${TOOLCHAIN_PREFIX})")
    endif ()
endif()

include(cmake/AddTor.cmake)

# onionpp library
add_library(onionpp SHARED include/onionpp.cpp)
set_target_properties(onionpp PROPERTIES LINKER_LANGUAGE CXX)
add_dependencies(onionpp tor)

# Test executable
add_executable(onionpp_test main.cpp include/onionpp.cpp)
add_dependencies(onionpp_test onionpp)
set_target_properties(onionpp_test PROPERTIES LINK_FLAGS_RELEASE -s)

# Python bindings
find_package(Python3 COMPONENTS Interpreter Development)
find_package(pybind11)
if(PYTHON AND pybind11_FOUND AND Python3_FOUND)
    message("Building python module")
    pybind11_add_module(onionpy SHARED python_binding.cpp)
    target_link_libraries(onionpy PRIVATE onionpp)
    add_dependencies(onionpy onionpp)
endif()

if(WINDOWS)
    message("Building for windows")
    set(onionpp_LINK_LIBRARIES -static -static-libgcc ${TOR_LIBRARY_PATHS} ws2_32 crypt32 shlwapi iphlpapi gdi32 pthread)
    target_link_libraries(onionpp ${onionpp_LINK_LIBRARIES})
    target_link_libraries(onionpp_test PRIVATE ${onionpp_LINK_LIBRARIES})
else()
    message("Building for linux")
    set(onionpp_LINK_LIBRARIES ${TOR_LIBRARIES} pthread m dl)
    target_link_libraries(onionpp ${onionpp_LINK_LIBRARIES})
    target_link_libraries(onionpp_test PRIVATE ${onionpp_LINK_LIBRARIES})
    add_executable(onionpp_so_test test_load_so.cpp)
    target_link_libraries(onionpp_so_test PRIVATE dl)
endif()

unset(WINDOWS CACHE)
unset(x86_64 CACHE)
unset(PYTHON CACHE)
