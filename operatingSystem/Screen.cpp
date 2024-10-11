#pragma warning(disable : 4996)
#include "Screen.h"
#include "FCFSScheduler.h"
#include <iostream>
#include <cstdlib> // For system()
#include <ctime>   // For rand()
#include <iomanip>
#include <chrono>
#include <sstream>

Screen::Screen() {}; // Instantiate the FCFS scheduler

std::string printCurrentTimestamp() {
    std::ostringstream oss;
    // Get the current time
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::tm local_time;
    localtime_s(&local_time, &now_time);

    oss << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");

    return oss.str();
}

void Screen::handleScreenS(const std::string& sInput, std::unordered_map<std::string, Process>& processMap) {
    system("CLS");
    std::string sCommand = "screen -s";
    std::string processName;

    // Get the process name
    int pos = sInput.find(sCommand);
    processName = sInput.substr(pos + 10); // Extracting process name

    // Get the current line of instruction
    int currentLine = rand() % 1000;

    // Get the total line of instruction
    int totalLine;
    do {
        totalLine = rand() % 1000;
    } while (totalLine < currentLine);

    // Generate arrivalTime and burstTime
    int arrivalTime = rand() % 100; // Placeholder for arrival time (can be changed)
    int burstTime = rand() % 50 + 1; // Placeholder for burst time, ensure non-zero

    // Instantiate the process object with the correct parameters
    Process newProcess(processName, currentLine, totalLine, arrivalTime, burstTime);

    // Store the process in the map
    processMap[processName] = newProcess;

    // Print the process details
    std::cout << "Process Name: " + newProcess.processName + "\n\n";
    std::cout << "Current instruction line: " << newProcess.currentLineOfInstruction << "\n";
    std::cout << "Lines of code: " << newProcess.totalLineOfInstruction << "\n";
    std::cout << "Timestamp: " << newProcess.getFormattedDate() << "\n\n";

    // Command loop for the screen session
    std::string screenInput;
    while (true) {
        std::cout << "Enter a command ('exit' to return to the main menu): ";
        std::getline(std::cin, screenInput);
        if (screenInput == "exit") {
            system("CLS");
            // main(); // TODO: Fix (no longer in main, but needs to instantiate main)
            break;  // Break out of the inner loop to return to the main menu
        }
        else {
            std::cout << "'" << screenInput << "' command is not recognized.\n";
        }
    }
    system("CLS");
};

void Screen::handleScreenR(const std::string& sInput, std::unordered_map<std::string, Process>& processMap) {
    system("CLS");
    std::string sCommand = "screen -r";
    std::string processName;

    // Get the process name
    int pos = sInput.find(sCommand);
    processName = sInput.substr(pos + 10); // Extracting process name

    // Check if the process exists
    if (processMap.find(processName) != processMap.end()) {
        Process existingProcess = processMap[processName];

        // Print the process details
        std::cout << "Reattaching to process:\n";
        std::cout << "Process Name: " + existingProcess.processName + "\n";
        std::cout << "Current instruction line: " << existingProcess.currentLineOfInstruction << " / " << existingProcess.totalLineOfInstruction << "\n";
        std::cout << "Timestamp: " << existingProcess.getFormattedDate() << "\n\n";

        // Command loop for the screen session
        std::string screenInput;
        while (true) {
            std::cout << "Enter a command ('exit' to return to the main menu): ";
            std::getline(std::cin, screenInput);
            if (screenInput == "exit") {
                system("CLS");
                // main(); // TODO: Fix (no longer in main, but needs to instantiate main)
                break;  // Break out of the inner loop to return to the main menu
            }
            else {
                std::cout << "'" << screenInput << "' command is not recognized.\n";
            }
        }
    }
    else {
        std::cout << "No detached screen process found with the name '" << processName << "'.\n\n";
    }
};

void Screen::handleScreenLS(std::unordered_map<std::string, Process>& processMap, FCFSScheduler scheduler) {
    system("CLS");
    std::cout << "------------------------------------------\n";
    std::cout << "Running processes:\n";
    // TODO: Print all active processes
    scheduler.printActiveProcesses();

    std::cout << "\n";

    std::cout << "Finished processes:\n";
    // TODO: Print all finished processes
    scheduler.printTerminatedProcesses();
    std::cout << "------------------------------------------\n";
}

