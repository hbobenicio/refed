cmake_minimum_required(VERSION 3.23)
project(demo LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    refed
    GIT_REPOSITORY "https://github.com/hbobenicio/refed.git"
    GIT_TAG        "c2a2e0d16bb6f7b3bd1a7d2573576ef126f4a97c"
)

FetchContent_MakeAvailable(refed)

add_executable(demo "main.cpp")

target_link_libraries(demo refed::refed)
