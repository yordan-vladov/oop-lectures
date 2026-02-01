cmake_minimum_required(VERSION 3.10)
project(CPPProject)

set(CMAKE_CXX_STANDARD 17)

# Добавяме библиотеката MathUtils
add_library(MathUtils STATIC
    src/MathUtils.cpp
)

# Добавяме директорията с header файлове
target_include_directories(MathUtils PUBLIC include)

# Създаваме изпълним файл
add_executable(MainApp
    src/main.cpp
)

# Линкваме библиотеката към main
target_link_libraries(MainApp PRIVATE MathUtils)
