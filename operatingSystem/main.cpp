// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <ctime>
#include <unordered_map>
#include "Process.h"

using namespace std;

std::unordered_map<std::string, Process> processMap; // To store active processes

int main()
{
    int nClear = 0;

    // Header text
    std::cout << "  __  ___   __  ___  ___  ___  _  _ \n";
    std::cout << " / _)/ __) /  \\(  ,\\(  _)/ __)( \\/ ) \n";
    std::cout << "( (_ \\__ \\( () )) _/ ) _)\\__ \\ \\  / \n";
    std::cout << " \\__)(___/ \\__/(_)  (___)(___/(__/ \n\n";

    std::cout << "By MP Group 10: Dela Cruz, Estrera, Fernandez, Villamiel\n\n";


    while (nClear == 0) {
        // Asking for text input
        std::string sInput;
        std::cout << "Please type in a command: ";
        std::getline(std::cin, sInput); // changed to getline to capture the entire input including spaces

        // Input validation and conditions
        if (sInput != "clear" && sInput != "exit") {
            if (sInput == "initialize" || sInput == "scheduler-test" || sInput == "scheduler-stop" || sInput == "report-util") {
                std::cout << "'" + sInput + "'" + " command recognized. Doing something.\n\n";
            }
            else if (sInput.find("screen -s") == 0) {
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
                        main();
                        break;  // Break out of the inner loop to return to the main menu
                    }
                    else {
                        std::cout << "'" << screenInput << "' command is not recognized.\n";
                    }
                }
                system("CLS");
            }
            else if (sInput.find("screen -r") == 0) {
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
                            main();
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

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
