cmake_minimum_required(VERSION 3.15...3.26)

project(
        onionpp
        LANGUAGES CXX
        VERSION 1.0.0
)

option(WINDOWS "Build for windows" OFF)
option(x86_64 "Build for x86_64" OFF)
option(EXAMPLE "Build example" OFF)
option(SO_TEST "Build .so load test" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(WINDOWS_PYTHON_DIRECTORY "")
set(TOOLCHAIN_PREFIX "")

set(CMAKE_CXX_FLAGS "-Wall -Wextra -fstack-protector-all")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(include)
file(GLOB_RECURSE ONIONPP_SOURCES
        include/onionpp/*.cpp
        include/onionpp/**/*.cpp
)

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
add_library(onionpp STATIC ${ONIONPP_SOURCES})
set_target_properties(onionpp PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(onionpp PUBLIC
        ONIONPP_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        ONIONPP_VERSION_MINOR=${PROJECT_VERSION_MINOR}
        ONIONPP_VERSION_PATCH=${PROJECT_VERSION_PATCH}
        ONIONPP_VERSION="${PROJECT_VERSION}"
)
add_dependencies(onionpp tor)

if (EXAMPLE)
    # Test executable
    add_executable(onionpp_test main.cpp ${ONIONPP_SOURCES})
    add_dependencies(onionpp_test onionpp)
    set_target_properties(onionpp_test PROPERTIES LINK_FLAGS_RELEASE -s)
endif ()

if (SO_TEST)
    add_executable(onionpp_so_test test_load_so.cpp)
    target_link_libraries(onionpp_so_test PRIVATE dl)
endif ()

if(WINDOWS)
    message("Building for windows")
    set(onionpp_LINK_LIBRARIES -static -static-libgcc ${TOR_LIBRARY_PATHS} ws2_32 crypt32 shlwapi iphlpapi gdi32 pthread)
    target_link_libraries(onionpp ${onionpp_LINK_LIBRARIES})
    if (EXAMPLE)
        target_link_libraries(onionpp_test PRIVATE ${onionpp_LINK_LIBRARIES} onionpp)
    endif ()
else()
    message("Building for linux")
    set(onionpp_LINK_LIBRARIES ${TOR_LIBRARIES} pthread m dl)
    target_link_libraries(onionpp ${onionpp_LINK_LIBRARIES})
    if (EXAMPLE)
        target_link_libraries(onionpp_test PRIVATE ${onionpp_LINK_LIBRARIES} onionpp)
    endif ()
endif()

unset(WINDOWS CACHE)
unset(x86_64 CACHE)
unset(EXAMPLE CACHE)
unset(SO_TEST CACHE)