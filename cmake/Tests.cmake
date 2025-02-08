include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG v1.15.2
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

add_executable(test test/test.cpp)
target_link_libraries(test onionpp GTest::gtest_main)
gtest_discover_tests(test)

if (SO_TEST)
    add_executable(onionpp_so_test test/test_load_so.cpp)
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        target_link_libraries(onionpp_so_test PRIVATE dl)
    endif ()
    add_dependencies(onionpp_so_test onionpp-shared)
endif ()
