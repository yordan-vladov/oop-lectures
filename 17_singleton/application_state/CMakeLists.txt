cmake_minimum_required(VERSION 3.16)

project(SingletonAppState LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SingletonAppState
    src/main.cpp
    src/ApplicationState.cpp
)

target_include_directories(SingletonAppState
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)
