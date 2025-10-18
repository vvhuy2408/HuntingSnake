// HuntingSnake.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "control.h"


void main() {
    int temp;
    fixConsoleWindow();
    startGame();

    thread t1(threadFunc); // Create thread for snake
    HANDLE handle_t1 = t1.native_handle(); // Take handle of thread

    while (1) {
        temp = toupper(_getch());

        if (STATE == 1) {
            if (temp == 'P') {
                pauseGame(handle_t1);
            }
            else if (temp == 27) { // ESC key
                exitGame(handle_t1);
                return;
            }
            else {
                ResumeThread(handle_t1);

                if ((temp != CHAR_LOCK) &&
                    (temp == 'D' || temp == 'A' || temp == 'W' || temp == 'S')) {

                    if (temp == 'D') CHAR_LOCK = 'A';
                    else if (temp == 'W') CHAR_LOCK = 'S';
                    else if (temp == 'S') CHAR_LOCK = 'W';
                    else CHAR_LOCK = 'D';

                    MOVING = temp;
                }
            }
        }
        else {
            if (temp == 'Y')
                startGame();
            else {
                exitGame(handle_t1);
                return;
            }
        }
    }
}

