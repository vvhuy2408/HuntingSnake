#pragma once
#ifndef CONTROL_H
#define CONTROL_H
#include "feature.h"

void growSnake(POINT nextHead);
void moveSnake(int dx, int dy);
void threadFunc();
int StepOnce(int dx, int dy);


#endif // CONTROL_H