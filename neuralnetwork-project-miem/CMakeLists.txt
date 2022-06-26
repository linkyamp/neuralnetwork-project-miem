make_minimum_required(VERSION 3.22)
project(neuralnetwork_project_miem)

set(CMAKE_CXX_STANDARD 14)

include_directories(neuralnetwork-project-miem)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(neuralnetwork_project_miem
        neuralnetwork-project-miem/ActivateFunction.cpp
        neuralnetwork-project-miem/ActivateFunction.h
        neuralnetwork-project-miem/Matrix.cpp
        neuralnetwork-project-miem/Matrix.h
        neuralnetwork-project-miem/NetWork.cpp
        neuralnetwork-project-miem/NetWork.h
        neuralnetwork-project-miem/source.cpp)
