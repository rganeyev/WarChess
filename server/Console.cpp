#include "Console.h"
#include "util/String.h"
#include "game/Settings.h"
#include <stdio.h>
#include <string.h>
#include <Windows.h>

extern uintptr_t serverThread;

Console::Console()
{
}

void Console::run()
{
	char buffer[512];
	while (true) {
		printf("> ");
		char* command = gets(buffer);
		if (!handleCommand(String(command, (unsigned int)strlen(command)))) {
			break;
		}
	}
}

bool Console::handleCommand( String command )
{
	if (command == String("exit", 4)) {
		TerminateThread((HANDLE)serverThread, 0);
		return false;
	} else if (command == String("reload", 6)) {
		if (settings.reload("server.xml")) {
			printf("Settings successfully reloaded\n");
		} else {
			printf("Failed to reload settings\n");
		}
	} else if (command == String("clear", 5)) {
		HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD hWrittenChars = 0;
		CONSOLE_SCREEN_BUFFER_INFO strConsoleInfo;
		COORD Home = {0, 0};

		GetConsoleScreenBufferInfo(hConsoleHandle, &strConsoleInfo);
		FillConsoleOutputCharacterA(hConsoleHandle, ' ', strConsoleInfo.dwSize.X * strConsoleInfo.dwSize.Y, Home, &hWrittenChars);
		SetConsoleCursorPosition(hConsoleHandle, Home);
	} else {
		printf("Unknown command\n");
	}

	return true;
}