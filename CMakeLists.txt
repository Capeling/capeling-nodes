cmake_minimum_required(VERSION 3.5.0)
project(capeling-nodes VERSION 0.1.0 LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE)

file(GLOB_RECURSE SOURCES src/*.cpp)

target_sources(${PROJECT_NAME} INTERFACE ${SOURCES})
target_include_directories(${PROJECT_NAME} INTERFACE include)
