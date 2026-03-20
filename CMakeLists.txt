cmake_minimum_required(VERSION 3.16)

project(qrcode VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenCV REQUIRED COMPONENTS core imgcodecs imgproc)

add_executable(qrcode
    src/main.cpp
    src/config_loader.cpp
    src/url_builder.cpp
    src/qr_generator.cpp
)

target_include_directories(qrcode PRIVATE include)

target_link_libraries(qrcode PRIVATE ${OpenCV_LIBS})

target_compile_definitions(qrcode PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
