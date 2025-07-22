cmake_minimum_required(VERSION 3.23)
project(demo LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    refed
    GIT_REPOSITORY "https://github.com/hbobenicio/refed.git"
    GIT_TAG        "f5a2aa2d958a1526a0740f062e0f03d8a256e38a"
)

FetchContent_MakeAvailable(refed)

add_executable(demo "main.cpp")

target_link_libraries(demo refed::refed)
