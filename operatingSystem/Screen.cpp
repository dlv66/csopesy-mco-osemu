#include "Screen.h"
#include <iostream>
#include <cstdlib> // For system()
#include <ctime>   // For rand()

Screen::Screen() {};

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

    // Instantiate the process object
    Process newProcess(processName, currentLine, totalLine);
    processMap[processName] = newProcess; // Store the process in the map

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