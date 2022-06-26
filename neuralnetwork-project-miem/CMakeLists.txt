cmake_minimum_required(VERSION 3.21)
project(neuralnetwork_project_miem)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(neuralnetwork_project_miem
        ActivateFunction.cpp
        ActivateFunction.h
        Matrix.cpp
        Matrix.h
        NetWork.cpp
        NetWork.h
        source.cpp)
