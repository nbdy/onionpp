include(ExternalProject)

set(ZLIB_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/zlib/bin)

if(WINDOWS)
    set(MAKE_ARGS "-fwin32/Makefile.gcc")
else()
    set(MAKE_ARGS "-j${CMAKE_BUILD_PARALLEL_LEVEL}")
endif()

ExternalProject_Add(zlib
        PREFIX zlib
        GIT_REPOSITORY https://github.com/madler/zlib
        GIT_PROGRESS 1
        GIT_TAG v1.3.1
        UPDATE_DISCONNECTED 1
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND ./configure --prefix=${ZLIB_BINARY_DIR} --static
        BUILD_COMMAND make clean && make ${MAKE_ARGS} PREFIX=${TOOLCHAIN_PREFIX} CFLAGS="-fPIC"
        INSTALL_COMMAND make install
)

ExternalProject_Get_Property(zlib SOURCE_DIR)
set(ZLIB_SOURCE_PATH ${SOURCE_DIR}/src)
include_directories(${ZLIB_SOURCE_PATH})
set(ZLIB_LIBRARY_PATH ${ZLIB_BINARY_DIR}/lib/libz.a)
set(ZLIB_LIBRARY_DIRECTORY ${ZLIB_BINARY_DIR}/lib)
set(ZLIB_LINK_LIBRARIES -L${ZLIB_LIBRARY_DIRECTORY} z)
