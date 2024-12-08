include(ExternalProject)

include(cmake/AddZLib.cmake)

if(${TOOLCHAIN_PREFIX} MATCHES "mingw32-$")
    set(CONFIGURE_PREFIX -m32 mingw)
elseif(${TOOLCHAIN_PREFIX} MATCHES "mingw64-$")
    set(CONFIGURE_PREFIX mingw64)
else()
    set(CONFIGURE_PREFIX linux-elf)
endif()
message("OpenSSL configure prefix: ${CONFIGURE_PREFIX}")

set(OPENSSL_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/openssl/bin)

ExternalProject_Add(openssl
        PREFIX openssl
        DEPENDS zlib
        GIT_REPOSITORY https://github.com/openssl/openssl
        GIT_PROGRESS 1
        GIT_TAG OpenSSL_1_0_2j
        GIT_SUBMODULES ""
        UPDATE_DISCONNECTED 1
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND
        ./Configure ${CONFIGURE_PREFIX} threads no-shared no-asm no-tests
        --cross-compile-prefix=${TOOLCHAIN_PREFIX}
        --prefix=${OPENSSL_BINARY_DIR}
        -static -static-libgcc -fPIC
        BUILD_COMMAND make clean && make
        INSTALL_COMMAND make install
)

ExternalProject_Get_Property(openssl SOURCE_DIR)
set(OPENSSL_SOURCE_PATH ${SOURCE_DIR}/src)
include_directories(${OPENSSL_SOURCE_PATH})
set(SSL_LIBRARY_PATH "${OPENSSL_BINARY_DIR}/lib/libssl.a")
set(CRYPTO_LIBRARY_PATH "${OPENSSL_BINARY_DIR}/lib/libcrypto.a")
set(OPENSSL_LIBRARY_DIRECTORY "${OPENSSL_BINARY_DIR}/lib")
set(OPENSSL_LINK_LIBRARIES -L${OPENSSL_LIBRARY_DIRECTORY} ssl crypto)
