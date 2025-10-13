# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(MatrixVisualization)

find_package(OpenSceneGraph REQUIRED COMPONENTS osg osgViewer osgDB osgGA)

add_executable(matrix_visualizer main.cpp)
target_link_libraries(matrix_visualizer
    osg
    osgViewer
    osgDB
    osgGA
)

