#include "MainConsole.h"

#include <iostream>
#include "ConsoleManager.h"

MainConsole::MainConsole() : AConsole("MainConsole")
{
    int processID = 0;
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
	while (true) {
		// Asking for text input
		std::string sInput;
		std::cout << "Please type in a command: ";
		std::getline(std::cin, sInput);

		// Input validation and conditions
		if (sInput != "clear" && sInput != "exit") {
			if (sInput == "initialize")
			{
				// read the config file (txt)
				// should be the first command to be called
				// all other commands wont work if this isnt called
			}
			else if (sInput == "report-util")
			{
				
			}
			else if (sInput.find("screen -s") == 0) {

				// gets the name of the screen and process
				std::string screenName = sInput.substr(10);

				std::shared_ptr<Process> process = std::make_shared<Process>(1, screenName);
				std::shared_ptr <BaseScreen> screen = std::make_shared <BaseScreen>(process, screenName);

				ConsoleManager::getInstance()->registerScreen(screen);
			}
			else if (sInput.find("screen -r") == 0) {

				std::string screenName = sInput.substr(10);
				ConsoleManager::getInstance()->switchToScreen(screenName);
			}
			else if (sInput == "screen -ls")
			{
				// TODO: Use GlobalScheduler singleton to get the list of active and terminated processes.
			}
			else if (sInput == "scheduler-stop")
			{
				// TODO: Use GlobalScheduler and make function to generate random processes
			}
			else if (sInput == "scheduler-test")
			{
				// TODO: Use GlobalScheduler and make function to generate random processes
			}
			else {
				std::cout << "'" + sInput + "'" + " command not recognized.\n\n";
			}
			sInput = "";
		}
		else if (sInput == "clear") {
			system("CLS");
			this->drawHeader();
		}
		else if (sInput == "exit") {
			ConsoleManager::getInstance()->exitApplication();
		}
	}
}