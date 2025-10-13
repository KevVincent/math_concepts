cmake_minimum_required(VERSION 3.10)
project(search_algorithms)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(data_structures STATIC
    src/data_structures/kd_tree.cpp
)
target_include_directories(data_structures PUBLIC include)

# add_executable(csv_format src/main.cpp)
# target_link_libraries(csv_format PRIVATE csv_formatter)
