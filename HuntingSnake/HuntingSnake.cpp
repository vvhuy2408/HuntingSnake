// HuntingSnake.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "control.h"


void main() {
    int temp;
    FixConsoleWindow();
    StartGame();

    thread t1(ThreadFunc); // Create thread for snake
    HANDLE handle_t1 = t1.native_handle(); // Take handle of thread

    while (1) {
        temp = toupper(_getch());

        if (STATE == 1) {
            if (temp == 'P') {
                PauseGame(handle_t1);
            }
            else if (temp == 27) { // ESC key
                ExitGame(handle_t1);
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
                StartGame();
            else {
                ExitGame(handle_t1);
                return;
            }
        }
    }
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
