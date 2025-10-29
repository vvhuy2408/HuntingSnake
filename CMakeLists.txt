cmake_minimum_required(VERSION 3.10)
project(HuntingSnake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tell CMake to use local SFML (the one you copied into your repo)
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(HuntingSnake
    main.cpp
    Design/effects.cpp
    Design/textureManager.cpp
    Design/interface.cpp
    Design/screenState.cpp
)

target_include_directories(HuntingSnake PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Design)

target_link_libraries(HuntingSnake
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)
