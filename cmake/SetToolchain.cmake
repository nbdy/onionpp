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
