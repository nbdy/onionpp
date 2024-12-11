include(ExternalProject)

include(cmake/AddZLib.cmake)

set(OPENSSL_CONFIGURE_PREFIX "")
if(OPENSSL_CONFIGURE_PREFIX STREQUAL "")
    if(TOOLCHAIN MATCHES "mingw")
        set(OPENSSL_CONFIGURE_PREFIX mingw64)
    else()
        set(OPENSSL_CONFIGURE_PREFIX linux-x86_64)
    endif()
endif ()

message("OpenSSL configure prefix: ${OPENSSL_CONFIGURE_PREFIX}")

set(OPENSSL_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/openssl/bin)

ExternalProject_Add(ext_openssl
        PREFIX openssl
        DEPENDS ext_zlib
        GIT_REPOSITORY https://github.com/openssl/openssl
        GIT_PROGRESS 1
        GIT_TAG OpenSSL_1_0_2j
        GIT_SUBMODULES ""
        UPDATE_DISCONNECTED 1
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND bash -c "
        CC=${CMAKE_C_COMPILER} \
        CXX=${CMAKE_CXX_COMPILER} \
        AR=${CMAKE_AR} \
        RANLIB=${CMAKE_RANLIB} \
        STRIP=${CMAKE_STRIP} \
        NM=${CMAKE_NM} \
        ./Configure ${OPENSSL_CONFIGURE_PREFIX} threads no-shared no-asm no-tests \
        --with-zlib-include=${ZLIB_BINARY_DIR}/include \
        --with-zlib-lib=${ZLIB_BINARY_DIR}/lib \
        --prefix=${OPENSSL_BINARY_DIR} \
        -static -static-libgcc \
        -fPIC \
        "
        BUILD_COMMAND make -j${CMAKE_BUILD_PARALLEL_LEVEL}
        INSTALL_COMMAND make install
)

ExternalProject_Get_Property(ext_openssl SOURCE_DIR)
set(OPENSSL_SOURCE_PATH ${SOURCE_DIR}/src)
include_directories(${OPENSSL_SOURCE_PATH})
set(SSL_LIBRARY_PATH "${OPENSSL_BINARY_DIR}/lib/libssl.a")
set(CRYPTO_LIBRARY_PATH "${OPENSSL_BINARY_DIR}/lib/libcrypto.a")
set(OPENSSL_LIBRARY_DIRECTORY "${OPENSSL_BINARY_DIR}/lib")
set(OPENSSL_INCLUDE_DIRECTORY "${OPENSSL_BINARY_DIR}/include")
set(OPENSSL_LINK_LIBRARIES -L${OPENSSL_LIBRARY_DIRECTORY} ssl crypto)
