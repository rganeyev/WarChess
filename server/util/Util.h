#pragma once

#include <Windows.h>

void printError(char* functionName, int errCode = GetLastError());

#define Assert(cond, func) if (!(cond)) printError(func)