include(ExternalProject)

set(ZLIB_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/zlib/bin)

if(WINDOWS)
    set(MAKE_ARGS "-fwin32/Makefile.gcc")
else()
    set(MAKE_ARGS "-j$(nproc)")
endif()

ExternalProject_Add(tor-zlib
        PREFIX tor-zlib
        GIT_REPOSITORY https://github.com/madler/zlib
        GIT_PROGRESS 1
        GIT_TAG v1.2.12
        UPDATE_DISCONNECTED 1
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND ./configure --prefix=${ZLIB_BINARY_DIR} --static
        BUILD_COMMAND make clean && make ${MAKE_ARGS} PREFIX=${TOOLCHAIN_PREFIX} CFLAGS="-fPIC"
        INSTALL_COMMAND make install
        )

ExternalProject_Get_Property(tor-zlib SOURCE_DIR)
set(ZLIB_SOURCE_PATH ${SOURCE_DIR}/src)
include_directories(${ZLIB_SOURCE_PATH})
set(ZLIB_LIBRARY ${ZLIB_BINARY_DIR}/lib/libz.a)
set(ZLIB_LIBRARIES ${ZLIB_BINARY_DIR}/lib)
