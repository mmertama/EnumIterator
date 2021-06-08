cmake_minimum_required(VERSION 3.5)

project(EnumIterator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(EnumIterator main.cpp)

target_sources(${PROJECT_NAME} PRIVATE
    main.cpp
    enumiterator.h)
