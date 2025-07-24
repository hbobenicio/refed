cmake_minimum_required(VERSION 3.23)
project(demo LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    refed
    GIT_REPOSITORY "https://github.com/hbobenicio/refed.git"
    GIT_TAG        "ed35e8ad8465a54a9b0d1931b2545d66dbf90bdf"
)

FetchContent_MakeAvailable(refed)

add_executable(demo "main.cpp")

target_link_libraries(demo refed::refed)
