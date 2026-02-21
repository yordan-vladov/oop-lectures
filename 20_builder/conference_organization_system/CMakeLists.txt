cmake_minimum_required(VERSION 3.16)
project(EventBuilderCMake LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(EventBuilder
    src/main.cpp
    src/domain/Event.cpp
    src/builder/WebinarBuilder.cpp
    src/builder/ConferenceBuilder.cpp
    src/builder/HybridEventBuilder.cpp
    src/builder/EventDirector.cpp
    src/builder/EventFluentBuilder.cpp
)

target_include_directories(EventBuilder PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
