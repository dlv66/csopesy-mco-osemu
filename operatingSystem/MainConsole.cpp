#include "MainConsole.h"

#include <iostream>
#include "ConsoleManager.h"

MainConsole::MainConsole() : AConsole("MainConsole")
{
}

void MainConsole::drawHeader()
{
    // print header
    std::cout << "  __  ___   __  ___  ___  ___  _  _ \n";
    std::cout << " / _)/ __) /  \\(  ,\\(  _)/ __)( \\/ ) \n";
    std::cout << "( (_ \\__ \\( () )) _/ ) _)\\__ \\ \\  / \n";
    std::cout << " \\__)(___/ \\__/(_)  (___)(___/(__/ \n\n";

    std::cout << "By MP Group 10: Dela Cruz, Estrera, Fernandez, Villamiel\n\n";
}

void MainConsole::onEnabled()
{
	this->drawHeader();
}

void MainConsole::display()
{
    
}

void MainConsole::process()
{
    // Asking for text input
    std::string sInput;
    std::cout << "Please type in a command: ";
    std::getline(std::cin, sInput); // changed to getline to capture the entire input including spaces

    // Input validation and conditions
    if (sInput != "clear" && sInput != "exit") {
        if (sInput == "initialize" || sInput == "scheduler-stop" || sInput == "report-util") {
            std::cout << "'" + sInput + "'" + " command recognized. Doing something.\n\n";
        }
        else if (sInput.find("screen -s") == 0) {
            //std::cout << "'" + sInput + "'" + " command recognized. Doing something.\n\n";
            std::shared_ptr<Process> process = std::make_shared<Process>();
            std::shared_ptr <BaseScreen> screen = std::make_shared <BaseScreen>(process, "samplescreen");
			ConsoleManager::getInstance()->registerScreen(screen);
			ConsoleManager::getInstance()->switchConsole("samplescreen");
        }
        else if (sInput.find("screen -r") == 0) {
            //std::cout << "'" + sInput + "'" + " command recognized. Doing something.\n\n";


            /*
            * FOLLOWING LINES ARE STRAIGHT FROM screen.cpp:
            * void Screen::handleScreenR(const std::string& sInput, std::unordered_map<std::string, Process>& processMap)
            * 
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
            */
        }
        else if (sInput == "screen -ls")
        {
            std::cout << "'" + sInput + "'" + " command recognized. Doing something.\n\n";

            /*
            * THE FOLLOWING LINES OF CODE ARE FROM screen.cpp:
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
            */
        }
        else if (sInput == "scheduler-test")
        {
            std::cout << "'" + sInput + "'" + " command recognized. Doing something.\n\n";
        }
        else {
            std::cout << "'" + sInput + "'" + " command not recognized.\n\n";
        }
    }
    else if (sInput == "clear") {
        system("CLS");
        this->drawHeader();
    }
    else if (sInput == "exit") {
        exit(0);
    }
}