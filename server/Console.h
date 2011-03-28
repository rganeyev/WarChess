#pragma once

class String;

class Console
{
public:
	Console();
	void run();

private:
	bool handleCommand(String command);
};