cmake_minimum_required(VERSION 3.10)
project(HuntingSnake)

set(CMAKE_CXX_STANDARD 17)
find_package(SFML 2.6 COMPONENTS system window graphics REQUIRED)

add_executable(HuntingSnake 
            main.cpp
            Design/effects.cpp
            Design/textureManager.cpp
            Design/interface.cpp
            Design/screenState.cpp
            HuntingSnake/feature.cpp
            HuntingSnake/gameLogic.cpp
            HuntingSnake/state.cpp
            HuntingSnake/globals.h)

target_link_libraries(HuntingSnake PRIVATE sfml-system sfml-window sfml-graphics sfml-audio)
target_include_directories(HuntingSnake PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Design)
