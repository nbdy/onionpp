include(ExternalProject)

include(cmake/AddOpenSSL.cmake)

set(LIBEVENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/libevent/bin)

ExternalProject_Add(ext_libevent
        PREFIX libevent
        DEPENDS ext_openssl
        GIT_REPOSITORY https://github.com/libevent/libevent
        GIT_PROGRESS 1
        GIT_TAG release-2.1.12-stable
        UPDATE_DISCONNECTED 1
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND bash -c "./autogen.sh && \
        CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} AR=${CMAKE_AR} \
        RANLIB=${CMAKE_RANLIB} STRIP=${CMAKE_STRIP} NM=${CMAKE_NM} \
        CFLAGS='-I${ZLIB_INCLUDE_DIRECTORY} -I${OPENSSL_INCLUDE_DIRECTORY}' \
        LDFLAGS='-L${ZLIB_LIBRARY_DIRECTORY} -L${OPENSSL_LIBRARY_DIRECTORY}' \
        ./configure --disable-samples --disable-libevent-regress \
        --enable-openssl --prefix=${LIBEVENT_BINARY_DIR} --host=${TOOLCHAIN}"
        BUILD_COMMAND make -j${CMAKE_BUILD_PARALLEL_LEVEL}
        INSTALL_COMMAND make install
        UPDATE_COMMAND ./autogen.sh
)

ExternalProject_Get_Property(ext_libevent SOURCE_DIR)
set(LIBEVENT_SOURCE_PATH ${SOURCE_DIR}/src)
include_directories(${LIBEVENT_SOURCE_PATH})
set(LIBEVENT_LIBRARY_PATH "${LIBEVENT_BINARY_DIR}/lib/libevent.a")
set(LIBEVENT_LIBRARY_DIRECTORY "${LIBEVENT_BINARY_DIR}/lib")
set(LIBEVENT_LINK_LIBRARIES -L${LIBEVENT_LIBRARY_DIRECTORY} event)
