include(ExternalProject)

set(ZLIB_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/zlib/bin)

ExternalProject_Add(ext_zlib
        PREFIX zlib
        GIT_REPOSITORY https://github.com/madler/zlib
        GIT_PROGRESS 1
        GIT_TAG v1.3.1
        UPDATE_DISCONNECTED 1
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND bash -c "
        CC=${CMAKE_C_COMPILER} \
        CXX=${CMAKE_CXX_COMPILER} \
        AR=${CMAKE_AR} \
        RANLIB=${CMAKE_RANLIB} \
        STRIP=${CMAKE_STRIP} \
        NM=${CMAKE_NM} \
        CFLAGS='-fPIC' \
        ./configure \
        --prefix=${ZLIB_BINARY_DIR} \
        --static"
        BUILD_COMMAND make -j${CMAKE_BUILD_PARALLEL_LEVEL}
        INSTALL_COMMAND make install
)

ExternalProject_Get_Property(ext_zlib SOURCE_DIR)
set(ZLIB_SOURCE_PATH ${SOURCE_DIR}/src)
include_directories(${ZLIB_SOURCE_PATH})
set(ZLIB_INCLUDE_DIRECTORY "${ZLIB_BINARY_DIR}/include")
set(ZLIB_LIBRARY_PATH "${ZLIB_BINARY_DIR}/lib/libz.a")
set(ZLIB_LIBRARY_DIRECTORY "${ZLIB_BINARY_DIR}/lib")
set(ZLIB_LINK_LIBRARIES -L${ZLIB_LIBRARY_DIRECTORY} z)
