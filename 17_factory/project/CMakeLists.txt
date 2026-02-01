cmake_minimum_required(VERSION 3.16)
project(FactoryGUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(FactoryGUI
    src/main.cpp
    src/core/UIFactory.cpp
    src/widgets/Button.cpp
    src/widgets/TextBox.cpp
    src/widgets/CheckBox.cpp
    src/widgets/Slider.cpp
)

target_include_directories(FactoryGUI PRIVATE include)
