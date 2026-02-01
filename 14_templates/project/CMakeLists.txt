cmake_minimum_required(VERSION 3.10)
project(TemplateArrayProject CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Директорията с header файловете
include_directories(include)

# Изпълним файл
add_executable(template_array
    src/main.cpp
)
