include(ExternalProject)
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
        --disable-zstd
        --disable-tool-name-check
        --enable-static-tor
        --enable-static-libevent
        --enable-static-openssl
        --enable-static-zlib
        --enable-pic
        --with-libevent-dir=${LIBEVENT_BINARY_DIR}
        --with-openssl-dir=${OPENSSL_BINARY_DIR}
        --with-zlib-dir=${ZLIB_BINARY_DIR}
        --prefix=${TOR_BINARY_DIR}
        BUILD_COMMAND make clean && LDFLAGS="-L${TOOLCHAIN_LIB_PATH}" CFLAGS="-I${TOOLCHAIN_LIB_PATH}" make -j$(nproc)
        INSTALL_COMMAND make install
        )

add_dependencies(tor libevent tor-zlib openssl)
ExternalProject_Get_Property(tor SOURCE_DIR)
set(TOR_SOURCE_PATH ${SOURCE_DIR}/src)
include_directories(${TOR_SOURCE_PATH})

set(TOR_LIBRARY_PATHS
        ${TOR_SOURCE_PATH}/core/libtor-app.a
        ${TOR_SOURCE_PATH}/lib/libtor-compress.a
        ${TOR_SOURCE_PATH}/lib/libtor-evloop.a
        ${TOR_SOURCE_PATH}/lib/libtor-tls.a
        ${TOR_SOURCE_PATH}/lib/libtor-crypt-ops.a
        ${TOR_SOURCE_PATH}/lib/libcurve25519_donna.a
        ${TOR_SOURCE_PATH}/lib/libtor-geoip.a
        ${TOR_SOURCE_PATH}/lib/libtor-time.a
        ${TOR_SOURCE_PATH}/lib/libtor-fs.a
        ${TOR_SOURCE_PATH}/lib/libtor-encoding.a
        ${TOR_SOURCE_PATH}/lib/libtor-sandbox.a
        ${TOR_SOURCE_PATH}/lib/libtor-net.a
        ${TOR_SOURCE_PATH}/lib/libtor-memarea.a
        ${TOR_SOURCE_PATH}/lib/libtor-math.a
        ${TOR_SOURCE_PATH}/lib/libtor-meminfo.a
        ${TOR_SOURCE_PATH}/lib/libtor-osinfo.a
        ${TOR_SOURCE_PATH}/lib/libtor-log.a
        ${TOR_SOURCE_PATH}/lib/libtor-lock.a
        ${TOR_SOURCE_PATH}/lib/libtor-fdio.a
        ${TOR_SOURCE_PATH}/lib/libtor-string.a
        ${TOR_SOURCE_PATH}/lib/libtor-term.a
        ${TOR_SOURCE_PATH}/lib/libtor-smartlist-core.a
        ${TOR_SOURCE_PATH}/lib/libtor-malloc.a
        ${TOR_SOURCE_PATH}/lib/libtor-wallclock.a
        ${TOR_SOURCE_PATH}/lib/libtor-err.a
        ${TOR_SOURCE_PATH}/lib/libtor-intmath.a
        ${TOR_SOURCE_PATH}/lib/libtor-ctime.a
        ${TOR_SOURCE_PATH}/lib/libtor-trace.a
        ${TOR_SOURCE_PATH}/lib/libtor-buf.a
        ${TOR_SOURCE_PATH}/lib/libtor-confmgt.a
        ${TOR_SOURCE_PATH}/lib/libtor-pubsub.a
        ${TOR_SOURCE_PATH}/lib/libtor-metrics.a
        ${TOR_SOURCE_PATH}/lib/libtor-dispatch.a
        ${TOR_SOURCE_PATH}/lib/libtor-version.a
        ${TOR_SOURCE_PATH}/lib/libtor-thread.a
        ${TOR_SOURCE_PATH}/lib/libtor-container.a
        ${TOR_SOURCE_PATH}/lib/libtor-process.a
        ${TOR_SOURCE_PATH}/lib/libtor-llharden.a
        ${TOR_SOURCE_PATH}/ext/keccak-tiny/libkeccak-tiny.a
        ${TOR_SOURCE_PATH}/ext/ed25519/ref10/libed25519_ref10.a
        ${TOR_SOURCE_PATH}/ext/ed25519/donna/libed25519_donna.a
        ${TOR_SOURCE_PATH}/trunnel/libor-trunnel.a
        ${OPENSSL_LIBRARIES}/libssl.a
        ${OPENSSL_LIBRARIES}/libcrypto.a
        ${LIBEVENT_LIBRARIES}/libevent.a
        ${ZLIB_LIBRARY}
        )

file(GLOB TOR_CLIENT_OBJECTS
        ${TOR_SOURCE_PATH}/feature/api/tor_api.o
        ${TOR_SOURCE_PATH}/feature/client/*.o
        )

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

        ${ZLIB_LIBRARY}
        )
