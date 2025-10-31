cmake_minimum_required(VERSION 3.31)
project(game)

set(CMAKE_CXX_STANDARD 26)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(game
    main.cpp
    src/Game.cpp
    src/Button.cpp
    src/Player.cpp
    src/Food.cpp
)
target_include_directories(game PRIVATE src)
target_link_libraries(game SFML::Graphics SFML::Window SFML::System)
