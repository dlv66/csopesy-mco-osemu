// main.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <ctime>
#include <unordered_map>
#include "Process.h"
#include "FCFSScheduler.h"
#include "Screen.h"
#include "Core.h"
#include <thread>

#include "ConsoleManager.h"
#include "GlobalScheduler.h"

int main()
{
    GlobalScheduler::initialize();
	ConsoleManager::initialize();

	bool running = true;
	while (running)
	{
		ConsoleManager::getInstance()->process();
		ConsoleManager::getInstance()->drawConsole();
	}

    //int nClear = 0;
    //Screen screen;

    //// Header text
    //std::cout << "  __  ___   __  ___  ___  ___  _  _ \n";
    //std::cout << " / _)/ __) /  \\(  ,\\(  _)/ __)( \\/ ) \n";
    //std::cout << "( (_ \\__ \\( () )) _/ ) _)\\__ \\ \\  / \n";
    //std::cout << " \\__)(___/ \\__/(_)  (___)(___/(__/ \n\n";

    //std::cout << "By MP Group 10: Dela Cruz, Estrera, Fernandez, Villamiel\n\n";

    //// Initialize the scheduler with a specific number of cores
    //int numberOfCores = 4;  // Adjust this based on requirements
    //FCFSScheduler scheduler(numberOfCores);
    //scheduler.instantiateCoreList(); // Initialize the cores

    //// Create 10 processes with 100 print commands each
    //for (int i = 0; i < 10; i++) {
    //    std::string processName = "Process_" + std::to_string(i + 1);
    //    int arrivalTime = rand() % 10;  // Random arrival time within the first 10 time units
    //    int burstTime = 5;              // Fixed burst time for simplicity
    //    int totalLines = 100;           // 100 print commands

    //    Process newProcess(processName, 0, totalLines, arrivalTime, burstTime);
    //    processMap[processName] = newProcess;
    //    scheduler.addProcess(newProcess);
    //}

    //// Run the FCFS scheduling in a separate thread
    //std::thread schedulerThread(&FCFSScheduler::runFCFS, &scheduler);

}
