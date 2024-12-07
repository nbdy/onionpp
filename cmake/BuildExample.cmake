if (EXAMPLE)
    add_executable(onionpp_test main.cpp ${ONIONPP_SOURCES})
    add_dependencies(onionpp_test onionpp)
    set_target_properties(onionpp_test PROPERTIES LINK_FLAGS_RELEASE -s)

    if(WINDOWS)
        target_link_libraries(onionpp_test PRIVATE ${onionpp_LINK_LIBRARIES} onionpp)
    else ()
        target_link_libraries(onionpp_test PRIVATE ${onionpp_LINK_LIBRARIES} onionpp)
    endif ()
endif ()

if (SO_TEST)
    add_executable(onionpp_so_test test_load_so.cpp)
    target_link_libraries(onionpp_so_test PRIVATE dl)
endif ()
