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
    set(VENV_PATH "${CMAKE_SOURCE_DIR}/.venv")

    find_package(Python)
    if (NOT EXISTS ${VENV_PATH})
        message(STATUS "Creating python virtualenv")
        execute_process(COMMAND ${Python_EXECUTABLE} "-m" "venv" ${VENV_PATH})
    endif ()

    set(ENV{VIRTUAL_ENV} ${VENV_PATH})
    set(Python_FIND_VIRTUALENV FIRST)
    unset (Python_EXECUTABLE)

    find_package(Python)
    message(STATUS "Found python: ${Python_EXECUTABLE}")

    include(cmake/AddPyBind11.cmake)

    set(STUB_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/stubs)

    pybind11_add_module(onionpy bindings/python.cpp)
    target_include_directories(onionpy PUBLIC ${ONIONPP_INSTALL_DIR}/include)
    add_dependencies(onionpy onionpp-static)
    target_link_libraries(onionpy PRIVATE onionpp-static pybind11::pybind11)
    install(TARGETS onionpy DESTINATION ${Python_SITELIB})

    add_custom_command(
            OUTPUT ${STUB_OUTPUT_DIR}/onionpy-stubs
            COMMAND ${Python_EXECUTABLE} -m pybind11_stubgen onionpy
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/stubs ${CMAKE_CURRENT_BINARY_DIR}
            DEPENDS onionpy
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    add_custom_target(onionpy-stubs ALL DEPENDS ${STUB_OUTPUT_DIR}/onionpy-stubs)
    install(FILES ${STUB_OUTPUT_DIR}/onionpy.pyi DESTINATION ${Python_SITELIB})
endif ()
