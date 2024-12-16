include(cmake/AddTor.cmake)

include_directories(include)
file(GLOB_RECURSE ONIONPP_SOURCES
        include/onionpp/*.cpp
        include/onionpp/**/*.cpp
)

add_library(onionpp OBJECT ${ONIONPP_SOURCES})
set_target_properties(onionpp PROPERTIES POSITION_INDEPENDENT_CODE 1)
target_compile_definitions(onionpp PUBLIC ONIONPP_VERSION="${PROJECT_VERSION}")
add_dependencies(onionpp ext_tor)

add_library(onionpp-shared SHARED $<TARGET_OBJECTS:onionpp>)
set_target_properties(onionpp-shared PROPERTIES LINKER_LANGUAGE CXX)
add_dependencies(onionpp-shared onionpp)

add_library(onionpp-static STATIC $<TARGET_OBJECTS:onionpp>)
set_target_properties(onionpp-static PROPERTIES LINKER_LANGUAGE CXX)
add_dependencies(onionpp-static onionpp)

install(TARGETS onionpp-static onionpp-shared LIBRARY DESTINATION lib)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include)

message(STATUS "CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(ONIONPP_LINK_LIBRARIES ${TOR_LINK_LIBRARIES} pthread -static -static-libgcc ws2_32 crypt32 shlwapi iphlpapi gdi32)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(ONIONPP_LINK_LIBRARIES ${TOR_LINK_LIBRARIES} pthread m dl)
else ()
    message(STATUS "System '${CMAKE_SYSTEM_NAME}' is not supported (yet)")
endif ()

target_link_libraries(onionpp PUBLIC ${ONIONPP_LINK_LIBRARIES})
target_link_libraries(onionpp-static PUBLIC ${ONIONPP_LINK_LIBRARIES})
target_link_libraries(onionpp-shared PRIVATE ${ONIONPP_LINK_LIBRARIES})

if (TORPP)
    add_executable(torpp main.cpp)
    add_dependencies(torpp onionpp-static)
    set_target_properties(torpp PROPERTIES LINK_FLAGS_RELEASE -s)
    target_include_directories(torpp PRIVATE ${CLI11_SOURCE_DIR}/include)
    target_link_libraries(torpp PRIVATE onionpp-static)
endif ()

if (SO_TEST)
    add_executable(onionpp_so_test test_load_so.cpp)
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        target_link_libraries(onionpp_so_test PRIVATE dl)
    endif ()
    add_dependencies(onionpp_so_test onionpp-shared)
endif ()

if (PYTHON_BINDINGS)
    find_package(Python)
    include(cmake/AddPyBind11.cmake)
    pybind11_add_module(onionpy bindings/python.cpp)
    target_include_directories(onionpy PUBLIC ${ONIONPP_INSTALL_DIR}/include)
    add_dependencies(onionpy onionpp-static)
    target_link_libraries(onionpy PRIVATE onionpp-static pybind11::pybind11)
    install(TARGETS onionpy DESTINATION ${Python_SITELIB})
endif ()
