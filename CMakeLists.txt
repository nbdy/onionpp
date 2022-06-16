cmake_minimum_required(VERSION 3.14..3.21)
# ------------
option(PYTHON_BINDINGS "Build python bindings" OFF)
# ------------
set(TOR_VERSION "0.4.6.10")
# ------------
include(ExternalProject)
include("cmake/HunterGate.cmake")
HunterGate(URL "https://github.com/cpp-pm/hunter/archive/v0.24.1.tar.gz" SHA1 "4942227a6e6f5e64414c55b97ef98609de199d18")
# ------------
project(onionpp)
# ------------
hunter_add_package(ZLIB)
find_package(ZLIB CONFIG REQUIRED)
# ------------
set(TOOLCHAIN_PATH "${ZLIB_ROOT}")
set(TOOLCHAIN_LIB_PATH "${TOOLCHAIN_PATH}/lib")
set(TOOLCHAIN_INCLUDE_PATH "${TOOLCHAIN_PATH}/include")
string(REGEX REPLACE "[-]gcc$" "-windres" TOOLCHAIN_RC "${CMAKE_C_COMPILER}")
message("RC: ${TOOLCHAIN_RC}")
# ------------
hunter_add_package(OpenSSL)
find_package(OpenSSL REQUIRED)
# ------------
hunter_add_package(Libevent)
find_package(Libevent CONFIG REQUIRED)
# ------------
ExternalProject_Add(tor
        PREFIX tor
        GIT_REPOSITORY https://git.torproject.org/tor.git
        GIT_PROGRESS 1
        GIT_TAG tor-${TOR_VERSION}
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
        --with-libevent-dir=${TOOLCHAIN_PATH}
        --with-openssl-dir=${TOOLCHAIN_PATH}
        --with-zlib-dir=${TOOLCHAIN_PATH}
        --prefix=${TOOLCHAIN_PATH}
        BUILD_COMMAND make clean && make -j$(nproc) LDFLAGS="-L${TOOLCHAIN_LIB_PATH}" CFLAGS="-I${TOOLCHAIN_LIB_PATH}"
        INSTALL_COMMAND make install
        )
# ------------
ExternalProject_Get_Property(tor SOURCE_DIR)
set(TOR_SOURCE_PATH ${SOURCE_DIR}/src)
include_directories(${TOR_SOURCE_PATH})
message("${TOR_SOURCE_PATH}")
# ------------
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

        -L${TOOLCHAIN_LIB_PATH}
        ssl crypto event z
        )
# ------------

add_library(onionpp onionpp.h)
set_target_properties(onionpp PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(onionpp ${TOR_LIBRARIES})
add_dependencies(onionpp tor)

# Test executable
add_executable(onionpp_test main.cpp onionpp.h)
target_link_libraries(onionpp_test ${TOR_LIBRARIES})
add_dependencies(onionpp_test tor)

if(PYTHON_BINDINGS)
    find_package(pybind11 CONFIG REQUIRED)
    pybind11_add_module(onionpy python_binding.cpp)
    add_dependencies(onionpy tor)
endif()
