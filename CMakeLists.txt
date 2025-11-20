cmake_minimum_required(VERSION 3.10)
project(lab8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)


# Добавляем исполняемый файл
add_executable(app main.cpp)