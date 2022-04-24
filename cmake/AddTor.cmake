include(cmake/AddLibEvent.cmake)
include(cmake/AddZLib.cmake)

set(TOR_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/tor/bin/)

ExternalProject_Add(tor
        PREFIX tor
        GIT_REPOSITORY https://git.torproject.org/tor.git
        GIT_PROGRESS 1
        GIT_TAG tor-0.4.6.10
        UPDATE_DISCONNECTED 1
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND ./autogen.sh && ./configure --host=${TOOLCHAIN}
        --disable-asciidoc
        --disable-manpage
        --disable-html-manual
        --disable-unittests
        --disable-lzma
        --disable-tool-name-check
        --enable-static-tor
        --enable-static-libevent
        --enable-static-openssl
        --enable-static-zlib
        --with-libevent-dir=${LIBEVENT_BINARY_DIR}
        --with-openssl-dir=${OPENSSL_BINARY_DIR}
        --with-zlib-dir=${ZLIB_BINARY_DIR}
        --prefix=${TOR_BINARY_DIR}
        BUILD_COMMAND make clean && make -j$(nproc) LDFLAGS="-L${TOOLCHAIN_LIB_PATH} -L${LIBEVENT_BINARY_DIR}/lib" CFLAGS="-I${TOOLCHAIN_LIB_PATH} -I${LIBEVENT_BINARY_DIR}/lib"
        INSTALL_COMMAND make install
        )

add_dependencies(tor libevent zlib openssl)
ExternalProject_Get_Property(tor SOURCE_DIR)
set(TOR_SOURCE_PATH ${SOURCE_DIR}/src)
include_directories(${TOR_SOURCE_PATH})

set(TOR_LIBRARIES
        -L${TOR_SOURCE_PATH}/core
        tor-app

        -L${TOR_SOURCE_PATH}/lib
        tor-compress
        tor-evloop
        tor-tls
        tor-crypt-ops
        curve25519_donna
        tor-geoip
        tor-time
        tor-fs
        tor-encoding
        tor-sandbox
        tor-net
        tor-memarea
        tor-math
        tor-meminfo
        tor-osinfo
        tor-log
        tor-lock
        tor-fdio
        tor-string
        tor-term
        tor-smartlist-core
        tor-malloc
        tor-wallclock
        tor-err
        tor-intmath
        tor-ctime
        tor-trace
        tor-buf
        tor-confmgt
        tor-pubsub
        tor-metrics
        tor-dispatch
        tor-version
        tor-thread
        tor-container
        tor-process
        tor-llharden

        -L${TOR_SOURCE_PATH}/ext/keccak-tiny
        keccak-tiny

        -L${TOR_SOURCE_PATH}/ext/ed25519/ref10
        ed25519_ref10

        -L${TOR_SOURCE_PATH}/ext/ed25519/donna
        ed25519_donna

        -L${TOR_SOURCE_PATH}/trunnel
        or-trunnel

        -L${OPENSSL_LIBRARIES}
        ssl crypto

        -L${LIBEVENT_LIBRARIES}
        event

        ${LIBLZMA_LIBRARIES}
        ${ZLIB_LIBRARIES}
        )
