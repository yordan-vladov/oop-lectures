cmake_minimum_required(VERSION 3.16)
project(StrategySort LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(StrategySort
    main.cpp
    src/SorterContext.cpp
    src/strategies/BubbleSort.cpp
    src/strategies/QuickSort.cpp
    src/strategies/MergeSort.cpp
    src/strategies/HeapSort.cpp
)

target_include_directories(StrategySort PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
