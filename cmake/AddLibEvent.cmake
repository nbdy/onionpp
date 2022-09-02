include(cmake/AddOpenSSL.cmake)

set(LIBEVENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/libevent/bin)

ExternalProject_Add(libevent
        PREFIX libevent
        DOWNLOAD_COMMAND wget https://github.com/libevent/libevent/releases/download/release-2.1.11-stable/libevent-2.1.11-stable.tar.gz && tar xf libevent-2.1.11-stable.tar.gz && rm libevent-2.1.11-stable.tar.gz && mv libevent-2.1.11-stable libevent
        UPDATE_DISCONNECTED 1
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND cd libevent-2.1.11-stable && ./autogen.sh && ./configure --enable-static --enable-shared --prefix=${LIBEVENT_BINARY_DIR} --host=${TOOLCHAIN}
        BUILD_COMMAND cd libevent-2.1.11-stable && make clean && make -j$(nproc)
        INSTALL_COMMAND cd libevent-2.1.11-stable && make clean && make install
        )

add_dependencies(libevent openssl)
ExternalProject_Get_Property(libevent SOURCE_DIR)
set(LIBEVENT_SOURCE_PATH ${SOURCE_DIR}/src)
include_directories(${LIBEVENT_SOURCE_PATH})
set(LIBEVENT_LIBRARY "${LIBEVENT_BINARY_DIR}/lib/libevent.a")
set(LIBEVENT_LIBRARIES "${LIBEVENT_BINARY_DIR}/lib")
