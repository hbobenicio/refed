cmake_minimum_required(VERSION 3.23)
project(demo LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    refed
    GIT_REPOSITORY "https://github.com/hbobenicio/refed.git"
    GIT_TAG        "e2437e88d4a629d549e9def6ee866438e9074a3c"
)

FetchContent_MakeAvailable(refed)

add_executable(demo "main.cpp")

target_link_libraries(demo refed::refed)
