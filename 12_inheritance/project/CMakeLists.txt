cmake_minimum_required(VERSION 3.10)
project(ShapesProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Директория с header-и
include_directories(include)

# Библиотека с фигурите (Rectangle + Circle)
add_library(Shapes STATIC
    src/Rectangle.cpp
    src/Circle.cpp
)

# Изпълним файл
add_executable(ShapesApp
    src/main.cpp
)

# Линкваме библиотеката към приложението
target_link_libraries(ShapesApp PRIVATE Shapes)
