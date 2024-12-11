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
        CONFIGURE_COMMAND bash -c "./autogen.sh && \
        CC=${CMAKE_C_COMPILER} \
        CXX=${CMAKE_CXX_COMPILER} \
        RC=${CMAKE_RC_COMPILER} \
        AR=${CMAKE_AR} \
        RANLIB=${CMAKE_RANLIB} \
        STRIP=${CMAKE_STRIP} \
        NM=${CMAKE_NM} \
        ./configure \
        --host=${TOOLCHAIN} \
        --disable-rust \
        --disable-asciidoc \
        --disable-manpage \
        --disable-html-manual \
        --disable-unittests \
        --disable-lzma \
        --disable-zstd \
        --disable-systemd \
        --disable-tool-name-check \
        --disable-option-checking \
        --disable-dependency-tracking \
        --disable-system-torrc \
        --enable-static-zlib \
        --with-zlib-dir=${ZLIB_BINARY_DIR} \
        --enable-static-libevent \
        --with-libevent-dir=${LIBEVENT_BINARY_DIR} \
        --enable-static-openssl \
        --with-openssl-dir=${OPENSSL_BINARY_DIR} \
        --enable-pic \
        --enable-static-tor \
        --prefix=${TOR_BINARY_DIR}"
        BUILD_COMMAND make -j${CMAKE_BUILD_PARALLEL_LEVEL}
        INSTALL_COMMAND make install
)

ExternalProject_Get_Property(ext_tor SOURCE_DIR)
set(TOR_SOURCE_PATH ${SOURCE_DIR})
set(TOR_LINK_LIBRARIES
        -L${TOR_SOURCE_PATH} tor

        ${OPENSSL_LINK_LIBRARIES}
        ${LIBEVENT_LINK_LIBRARIES}
        ${ZLIB_LINK_LIBRARIES}
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(TOR_LINK_LIBRARIES ${TOR_LINK_LIBRARIES} cap)
endif ()