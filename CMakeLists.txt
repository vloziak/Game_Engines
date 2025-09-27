cmake_minimum_required(VERSION 3.16)
project(GameEnginesAssignment1)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 3 REQUIRED COMPONENTS Graphics)

find_package(glm REQUIRED)

add_executable(assignment1 main.cpp)

target_link_libraries(assignment1 SFML::Graphics glm::glm)
