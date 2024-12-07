include(ExternalProject)

include(cmake/AddOpenSSL.cmake)

set(LIBEVENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/libevent/bin)

ExternalProject_Add(libevent
        PREFIX libevent
        DEPENDS openssl
        GIT_REPOSITORY https://github.com/libevent/libevent
        GIT_PROGRESS 1
        GIT_TAG release-2.1.12-stable
        UPDATE_DISCONNECTED 1
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND ./autogen.sh && ./configure
        --disable-samples
        --disable-libevent-regress
        --enable-openssl
        --with-openssl-dir=${OPENSSL_BINARY_DIR}
        --prefix=${LIBEVENT_BINARY_DIR}
        --host=${TOOLCHAIN}
        BUILD_COMMAND make -j${CMAKE_BUILD_PARALLEL_LEVEL}
        INSTALL_COMMAND make install
)

ExternalProject_Get_Property(libevent SOURCE_DIR)
set(LIBEVENT_SOURCE_PATH ${SOURCE_DIR}/src)
include_directories(${LIBEVENT_SOURCE_PATH})
set(LIBEVENT_LIBRARY "${LIBEVENT_BINARY_DIR}/lib/libevent.a")
set(LIBEVENT_LIBRARIES "${LIBEVENT_BINARY_DIR}/lib")
