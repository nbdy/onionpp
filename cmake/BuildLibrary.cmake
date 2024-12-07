include(cmake/AddTor.cmake)

include_directories(include)
file(GLOB_RECURSE ONIONPP_SOURCES include/onionpp/*.cpp include/onionpp/**/*.cpp)

add_library(onionpp OBJECT ${ONIONPP_SOURCES})
set_target_properties(onionpp PROPERTIES POSITION_INDEPENDENT_CODE 1)
target_compile_definitions(onionpp PUBLIC ONIONPP_VERSION="${PROJECT_VERSION}")
target_include_directories(onionpp PUBLIC ${TOR_SOURCE_PATH})
add_dependencies(onionpp tor)

add_library(onionpp-shared SHARED $<TARGET_OBJECTS:onionpp>)
set_target_properties(onionpp-shared PROPERTIES LINKER_LANGUAGE CXX)
add_dependencies(onionpp-shared onionpp)

add_library(onionpp-static STATIC $<TARGET_OBJECTS:onionpp>)
set_target_properties(onionpp-static PROPERTIES LINKER_LANGUAGE CXX)
add_dependencies(onionpp-static onionpp)



install(TARGETS onionpp-static onionpp-shared LIBRARY DESTINATION lib)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include)

if(WINDOWS)
    set(onionpp_LINK_LIBRARIES -static -static-libgcc ${TOR_LIBRARY_PATHS} ws2_32 crypt32 shlwapi iphlpapi gdi32 pthread)
else ()
    set(onionpp_LINK_LIBRARIES ${TOR_LIBRARIES} pthread m dl)
endif ()

target_link_libraries(onionpp PUBLIC ${onionpp_LINK_LIBRARIES})
