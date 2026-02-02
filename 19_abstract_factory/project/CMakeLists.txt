cmake_minimum_required(VERSION 3.16)
project(AbstractFactoryGUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(AbstractFactoryGUI
    src/main.cpp
    src/factories/WindowsFactory.cpp
    src/factories/MacFactory.cpp
    src/products/WindowsButton.cpp
    src/products/WindowsCheckBox.cpp
    src/products/WindowsTextBox.cpp
    src/products/MacButton.cpp
    src/products/MacCheckBox.cpp
    src/products/MacTextBox.cpp
)

target_include_directories(AbstractFactoryGUI PRIVATE include)
