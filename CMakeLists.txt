cmake_minimum_required(VERSION 3.4)
project(onionpp LANGUAGES CXX)

option(PYTHON "Build python binding" ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Pull tor and set it up as a dependency
include(ExternalProject)
ExternalProject_Add(tor
        PREFIX tor
        GIT_REPOSITORY https://git.torproject.org/tor.git
        GIT_PROGRESS 1
        GIT_TAG tor-0.4.7.4-alpha
        UPDATE_DISCONNECTED 1
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND ./autogen.sh &&
            ./configure --disable-asciidoc
                        --disable-manpage
                        --disable-html-manual
                        --disable-unittests
                        --enable-pic
        BUILD_COMMAND make -j$(nproc)
        INSTALL_COMMAND ""
        )


ExternalProject_Get_Property(tor SOURCE_DIR)
set(TOR_SOURCE_PATH ${SOURCE_DIR}/src)
include_directories(${TOR_SOURCE_PATH})

# onionpp cmake script
include(FindOpenSSL)
include(FindLibLZMA)
include(FindZLIB)
include(cmake/FindLibEvent.cmake)
include(cmake/FindZSTD.cmake)

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

        ${OPENSSL_LIBRARIES}
        ${LIBLZMA_LIBRARIES}
        ${LIBEVENT_LIBRARY}
        ${ZLIB_LIBRARIES}
        ${ZSTD_LIBRARIES}
        )

# Python bindings
if(PYTHON)
    include(FindPython3)
    find_package(Python COMPONENTS Interpreter Development REQUIRED)
    find_package(pybind11 CONFIG REQUIRED)
    pybind11_add_module(onionpp MODULE python_binding.cpp)
    target_link_libraries(onionpp PRIVATE ${TOR_LIBRARIES})
    set_target_properties(onionpp PROPERTIES POSITION_INDEPENDENT_CODE ON)
    install(TARGETS onionpp
            COMPONENT python
            DESTINATION ${Python3_SITELIB}
            LIBRARY DESTINATION ${Python3_SITELIB})
    add_dependencies(onionpp tor)
endif()

# Test executable
add_executable(onionpp_test main.cpp onionpp.h)
add_dependencies(onionpp_test tor)
target_link_libraries(onionpp_test PRIVATE ${TOR_LIBRARIES} pthread m)
