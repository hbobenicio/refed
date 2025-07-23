# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY "https://github.com/google/googletest.git"
    GIT_TAG        "v1.17.0"
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(refed-tests-unit
    "defer.unittest.cpp"
)

target_link_libraries(refed-tests-unit
    GTest::gtest_main
    refed
)

include(GoogleTest)

gtest_discover_tests(refed-tests-unit)
