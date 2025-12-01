#ifndef SCREENSTATE_HPP
#define SCREENSTATE_HPP
#include <iostream>
#include <stack>

enum ScreenState {
    StartMenu,
    MainMenu,
    LoadGame,
    HowTo,
    About,
    InGame
};
// các hàm chuyển
void changeScreen(std::stack<ScreenState>& stack, ScreenState newScreen);
void goBack(std::stack<ScreenState>& stack);

#endif
