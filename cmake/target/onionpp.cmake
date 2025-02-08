include(cmake/dependencies/Tor.cmake)

include_directories(include)
file(GLOB_RECURSE ONIONPP_SOURCES
        src/onionpp/*.cpp
        src/onionpp/**/*.cpp
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

target_link_libraries(onionpp-static PUBLIC ${ONIONPP_LINK_LIBRARIES})
target_link_libraries(onionpp-shared PRIVATE ${ONIONPP_LINK_LIBRARIES})
