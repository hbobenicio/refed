cmake_minimum_required(VERSION 3.23)
project(demo LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    refed
    GIT_REPOSITORY https://github.com/hbobenicio/refed.git
    GIT_TAG        fb7bcf690377913053b4dc8b7abc04a370540f6c # 0.0.1
)

FetchContent_MakeAvailable(refed)

add_executable(demo "main.cpp")

target_link_libraries(demo refed::refed)
