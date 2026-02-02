cmake_minimum_required(VERSION 3.16)

project(SolidPayments LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SolidPayments
    src/main.cpp
    src/CashPayment.cpp
    src/CardPayment.cpp
    src/ConsoleReceiptPrinter.cpp
    src/PaymentProcessor.cpp
)

target_include_directories(SolidPayments PRIVATE ${CMAKE_SOURCE_DIR}/include)
