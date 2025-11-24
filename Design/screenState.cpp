#include "screenState.hpp"

void changeScreen(std::stack<ScreenState>& stack, ScreenState newScreen)
{
    stack.push(newScreen);
}

void goBack(std::stack<ScreenState>& stack)
{
    if (stack.size() > 1)
        stack.pop(); 
}
