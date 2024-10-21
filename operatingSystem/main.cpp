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

using namespace std;

std::unordered_map<std::string, Process> processMap; // To store active processes, GLOBAL VARIABLE AT THE MOMENT

int main()
{
    int nClear = 0;
    Screen screen;

    // Header text
    std::cout << "  __  ___   __  ___  ___  ___  _  _ \n";
    std::cout << " / _)/ __) /  \\(  ,\\(  _)/ __)( \\/ ) \n";
    std::cout << "( (_ \\__ \\( () )) _/ ) _)\\__ \\ \\  / \n";
    std::cout << " \\__)(___/ \\__/(_)  (___)(___/(__/ \n\n";

    std::cout << "By MP Group 10: Dela Cruz, Estrera, Fernandez, Villamiel\n\n";

    // Initialize the scheduler with a specific number of cores
    int numberOfCores = 4;  // Adjust this based on requirements
    FCFSScheduler scheduler(numberOfCores);
    scheduler.instantiateCoreList(); // Initialize the cores

    // Create 10 processes with 100 print commands each
    for (int i = 0; i < 10; i++) {
        int pid = i;
        std::string processName = "Process_" + std::to_string(i + 1);
        int arrivalTime = rand() % 10;  // Random arrival time within the first 10 time units
        int burstTime = 5;              // Fixed burst time for simplicity
        int totalLines = 100;           // 100 print commands

        Process newProcess(pid, processName, 0, totalLines, arrivalTime, burstTime);
        processMap[processName] = newProcess;
        scheduler.addProcess(newProcess);
    }

    // Run the FCFS scheduling in a separate thread
    std::thread schedulerThread(&FCFSScheduler::runFCFS, &scheduler);


    while (nClear == 0) {
        // Asking for text input
        std::string sInput;
        std::cout << "Please type in a command: ";
        std::getline(std::cin, sInput);

        // Input validation and conditions
        if (sInput != "clear" && sInput != "exit") {
            if (sInput == "initialize" || sInput == "scheduler-stop" || sInput == "report-util") {
                std::cout << "'" + sInput + "'" + " command recognized. Doing something.\n\n";
            }
            else if (sInput.find("screen -s") == 0) {
                screen.handleScreenS(sInput, processMap); // Call the function for "screen -s"
            }
            else if (sInput.find("screen -r") == 0) {
                screen.handleScreenR(sInput, processMap); // Call the function for "screen -r"
            }
            else if(sInput == "screen -ls")
            {
                screen.handleScreenLS(processMap, scheduler); // Call the function for "screen -ls"
            }
            else if(sInput == "scheduler-test")
            {
                // std::thread schedulerThread(&FCFSScheduler::runFCFS, &scheduler);
            }
            else {
                std::cout << "'" + sInput + "'" + " command NOT recognized. Please try again.\n\n";
            }
        }
        else if (sInput == "clear") {
            system("CLS");
            main();
        }
        else if (sInput == "exit") {
            nClear = 1;
        }
    };

    if (nClear == 1) {
        exit(0);
    }  
}
