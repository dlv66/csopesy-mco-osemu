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

using namespace std;

std::unordered_map<std::string, Process> processMap; // To store active processes

int main()
{
    int nClear = 0;
    Screen screen;
    FCFSScheduler scheduler(4); // 4 cores

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
                screen.handleScreenS(sInput, processMap); // Call the function for "screen -s"
            }
            else if (sInput.find("screen -r") == 0) {
                screen.handleScreenR(sInput, processMap); // Call the function for "screen -r"
            }
            else if(sInput == "screen -ls")
            {
                screen.handleScreenLS(processMap, scheduler); // Call the function for "screen -ls"
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
