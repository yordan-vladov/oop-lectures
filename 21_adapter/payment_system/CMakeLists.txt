cmake_minimum_required(VERSION 3.16)

project(PaymentAdapterProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(payment_app
    src/main.cpp

    src/external/PayPalAPI.cpp
    src/external/StripeAPI.cpp
    src/external/CryptoAPI.cpp

    src/payment/PaymentProcessorFactory.cpp
    src/payment/PayPalAdapter.cpp
    src/payment/StripeAdapter.cpp
    src/payment/CryptoAdapter.cpp
)

target_include_directories(payment_app PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)