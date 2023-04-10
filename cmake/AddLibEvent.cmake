include(ExternalProject)
include(cmake/AddOpenSSL.cmake)

set(LIBEVENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/libevent/bin)

ExternalProject_Add(libevent
        PREFIX libevent
        GIT_REPOSITORY https://github.com/libevent/libevent
        GIT_PROGRESS 1
        GIT_TAG release-2.1.11-stable
        UPDATE_DISCONNECTED 1
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND ./autogen.sh && ./configure
        --prefix=${LIBEVENT_BINARY_DIR}
        --host=${TOOLCHAIN}
        BUILD_COMMAND make -j$(nproc)
        INSTALL_COMMAND make install
        )

add_dependencies(libevent openssl)
ExternalProject_Get_Property(libevent SOURCE_DIR)
set(LIBEVENT_SOURCE_PATH ${SOURCE_DIR}/src)
include_directories(${LIBEVENT_SOURCE_PATH})
set(LIBEVENT_LIBRARY "${LIBEVENT_BINARY_DIR}/lib/libevent.a")
set(LIBEVENT_LIBRARIES "${LIBEVENT_BINARY_DIR}/lib")
