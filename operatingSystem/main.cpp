// main.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <ctime>
#include <unordered_map>
#include "Process.h"
#include "FCFSScheduler.h"
#include "RRScheduler.h"
#include "Core.h"
#include <thread>

#include "ConsoleManager.h"
#include "GlobalScheduler.h"

int main()
{
	Initialize init;
	ConsoleManager::initialize();
	bool running = true;
	while (running)
	{
		ConsoleManager::getInstance()->process();
		ConsoleManager::getInstance()->drawConsole();

	}

}
