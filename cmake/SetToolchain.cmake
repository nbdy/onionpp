set(TOOLCHAIN "" CACHE STRING "Toolchain to use (e.g. x86_64-w64-mingw32)")
set(TOOLCHAIN_SUFFIX "" CACHE STRING "Toolchain suffix to use (e.g. -posix)")
set(TOOLCHAIN_LIB_PATH "")

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(TOOLCHAIN "x86_64-w64-mingw32") # Set to mingw by default if we target windows
    set(TOOLCHAIN_SUFFIX "-posix")
endif ()

if (TOOLCHAIN STREQUAL "")
    message(STATUS "Using default toolchain")
else ()
    message(STATUS "Using toolchain: ${TOOLCHAIN}")
    set(CMAKE_C_COMPILER "${TOOLCHAIN}-gcc${TOOLCHAIN_SUFFIX}")
    set(CMAKE_CXX_COMPILER "${TOOLCHAIN}-g++${TOOLCHAIN_SUFFIX}")
    set(CMAKE_RC_COMPILER "${TOOLCHAIN}-windres${TOOLCHAIN_SUFFIX}")
    set(CMAKE_AR "${TOOLCHAIN}-gcc-ar${TOOLCHAIN_SUFFIX}")
    set(CMAKE_RANLIB "${TOOLCHAIN}-gcc-ranlib")
    set(CMAKE_STRIP "${TOOLCHAIN}-gcc-strip${TOOLCHAIN_SUFFIX}")
    set(CMAKE_NM "${TOOLCHAIN}-gcc-nm${TOOLCHAIN_SUFFIX}")
endif ()

unset(TOOLCHAIN CACHE)
unset(TOOLCHAIN_SUFFIX CACHE)
