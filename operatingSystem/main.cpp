// main.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <ctime>
#include <unordered_map>
#include "Process.h"
#include "FCFSScheduler.h"
#include "Core.h"
#include <thread>

#include "ConsoleManager.h"
#include "GlobalScheduler.h"

int main()
{
	Initialize init;
	ConsoleManager::initialize();
    //TODO: make a process initializer
	bool running = true;
	while (running)
	{
        //GlobalScheduler::getInstance()->start();
		ConsoleManager::getInstance()->process();
		ConsoleManager::getInstance()->drawConsole();

        //TODO: add cpu cycles here ?
        // i dont know if this is the right position
	}

}
