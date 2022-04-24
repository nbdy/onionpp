if(${TOOLCHAIN_PREFIX} MATCHES "mingw32-$")
    set(CONFIGURE_PREFIX mingw)
elseif(${TOOLCHAIN_PREFIX} MATCHES "mingw64-$")
    set(CONFIGURE_PREFIX mingw64)
else()
    set(CONFIGURE_PREFIX linux-generic32)
endif()

set(OPENSSL_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/openssl/bin)

ExternalProject_Add(openssl
        PREFIX openssl
        GIT_REPOSITORY https://github.com/openssl/openssl
        GIT_PROGRESS 1
        GIT_TAG OpenSSL_1_0_2j
        UPDATE_DISCONNECTED 1
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND export LDFLAGS="-static -static-libgcc -L/usr/x86_64-w64-mingw32/lib/" &&
        ./Configure ${CONFIGURE_PREFIX} no-shared no-asm
        --cross-compile-prefix=${TOOLCHAIN_PREFIX}
        --prefix=${OPENSSL_BINARY_DIR}
        -static -static-libgcc
        BUILD_COMMAND make
        INSTALL_COMMAND make install
        )

ExternalProject_Get_Property(openssl SOURCE_DIR)
set(OPENSSL_SOURCE_PATH ${SOURCE_DIR}/src)
include_directories(${OPENSSL_SOURCE_PATH})
set(OPENSSL_LIBRARY "${OPENSSL_BINARY_DIR}/lib/libssl.a")
set(CRYPTO_LIBRARY "${OPENSSL_BINARY_DIR}/lib/libcrypto.a")
set(OPENSSL_LIBRARIES "${OPENSSL_BINARY_DIR}/lib")