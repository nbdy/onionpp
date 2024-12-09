include(ExternalProject)

include(cmake/AddLibEvent.cmake)

set(TOR_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/tor/bin/)

ExternalProject_Add(ext_tor
        PREFIX tor
        DEPENDS ext_openssl ext_libevent ext_zlib
        GIT_REPOSITORY https://git.torproject.org/tor.git
        GIT_PROGRESS 1
        GIT_SUBMODULES ""
        GIT_TAG tor-0.4.8.13
        UPDATE_DISCONNECTED 1
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND ./autogen.sh &&
        ./configure
        --host=${TOOLCHAIN}
        --disable-rust
        --disable-asciidoc
        --disable-manpage
        --disable-html-manual
        --disable-unittests
        --disable-lzma
        --disable-zstd
        --disable-systemd
        --disable-tool-name-check
        --enable-static-tor
        --enable-static-zlib
        --with-zlib-dir=${ZLIB_BINARY_DIR}
        --enable-static-libevent
        --with-libevent-dir=${LIBEVENT_BINARY_DIR}
        --enable-static-openssl
        --with-openssl-dir=${OPENSSL_BINARY_DIR}
        --enable-pic
        # TODO: --enable-android
        --prefix=${TOR_BINARY_DIR}
        BUILD_COMMAND make clean &&
        LDFLAGS="-L${TOOLCHAIN_LIB_PATH}" CFLAGS="-I${TOOLCHAIN_LIB_PATH}" make -j${CMAKE_BUILD_PARALLEL_LEVEL}
        INSTALL_COMMAND make install
)

ExternalProject_Get_Property(ext_tor SOURCE_DIR)
set(TOR_SOURCE_PATH ${SOURCE_DIR})
set(TOR_LINK_LIBRARIES
        -L${TOR_SOURCE_PATH}/core tor-app

        -L${TOR_SOURCE_PATH}/lib
        tor-compress tor-evloop tor-tls tor-crypt-ops curve25519_donna
        tor-geoip tor-time tor-fs tor-encoding tor-sandbox tor-net tor-memarea
        tor-math tor-meminfo tor-osinfo tor-log tor-lock tor-fdio tor-string
        tor-term tor-smartlist-core tor-malloc tor-wallclock tor-err tor-intmath
        tor-ctime tor-trace tor-buf tor-confmgt tor-pubsub tor-metrics tor-dispatch
        tor-version tor-thread tor-container tor-process tor-llharden

        -L${TOR_SOURCE_PATH}/ext/keccak-tiny keccak-tiny
        -L${TOR_SOURCE_PATH}/ext/ed25519/ref10 ed25519_ref10
        -L${TOR_SOURCE_PATH}/ext/ed25519/donna ed25519_donna
        -L${TOR_SOURCE_PATH}/trunnel or-trunnel

        ${OPENSSL_LINK_LIBRARIES}
        ${LIBEVENT_LINK_LIBRARIES}
        ${ZLIB_LINK_LIBRARIES}

        cap
)
