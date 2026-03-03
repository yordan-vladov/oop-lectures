cmake_minimum_required(VERSION 3.16)
project(ObserverNews LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ObserverNews
    app/main.cpp
    src/Observer/NewsAgency.cpp
    src/Observers/TVChannel.cpp
    src/Observers/Radio.cpp
    src/Observers/MobileApp.cpp
    src/Observers/Website.cpp
)

target_include_directories(ObserverNews PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
