cmake_minimum_required(VERSION 3.4)
project(onionpp LANGUAGES CXX)

option(PYTHON "Build python binding" OFF)
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

# Python bindings
if(PYTHON)
    set(PYTHON_INCLUDE_DIRS ${WINDOWS_PYTHON_DIRECTORY} CACHE INTERNAL "Cross python include path")
    set(PYTHON_MODULE_EXTENSION ".so" CACHE INTERNAL "Cross python lib extension")
    set(PYTHONLIBS_FOUND TRUE CACHE INTERNAL "")
    add_subdirectory(pybind11)

    pybind11_add_module(onionpp MODULE python_binding.cpp)
    target_link_libraries(onionpp PRIVATE ${TOR_LIBRARIES})
    set_target_properties(onionpp PROPERTIES POSITION_INDEPENDENT_CODE ON)

    # install(TARGETS onionpp COMPONENT python DESTINATION ${Python3_SITELIB} LIBRARY DESTINATION ${Python3_SITELIB})

    add_dependencies(onionpp tor)
endif()

# Test executable
add_executable(onionpp_test main.cpp onionpp.h)
add_dependencies(onionpp_test tor)

if(WINDOWS)
    target_link_libraries(onionpp_test PRIVATE -static -static-libgcc ${TOR_LIBRARIES} ws2_32 crypt32 shlwapi iphlpapi gdi32)
else()
    target_link_libraries(onionpp_test PRIVATE ${TOR_LIBRARIES} pthread m)
endif()
