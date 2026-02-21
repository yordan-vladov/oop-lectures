cmake_minimum_required(VERSION 3.16)
project(DecoratorTextEditor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(decorator_text_editor
    src/main.cpp
    src/PlainText.cpp
    src/TextDecorator.cpp
    src/BoldDecorator.cpp
    src/ItalicDecorator.cpp
    src/UnderlineDecorator.cpp
    src/ShadowDecorator.cpp
)

target_include_directories(decorator_text_editor PRIVATE include)
