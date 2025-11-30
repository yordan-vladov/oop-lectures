cmake_minimum_required(VERSION 3.10)
project(DevicesAndInterfaces CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Папка с хедърите
include_directories(include)

# Източници
set(SOURCES
    src/Phone.cpp
    src/Laptop.cpp
    src/main.cpp
)

add_executable(DevicesAndInterfaces ${SOURCES})
