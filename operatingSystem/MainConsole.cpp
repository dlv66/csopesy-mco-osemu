#include "MainConsole.h"

#include <iostream>

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
            std::cout << "'" + sInput + "'" + " command recognized. Doing something.\n\n";
        }
        else if (sInput.find("screen -r") == 0) {
            std::cout << "'" + sInput + "'" + " command recognized. Doing something.\n\n";
        }
        else if (sInput == "screen -ls")
        {
            std::cout << "'" + sInput + "'" + " command recognized. Doing something.\n\n";
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