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
