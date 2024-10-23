#include "BaseScreen.h"

#include <iostream>
#include <ostream>

#include "MainConsole.h"
#include "ConsoleManager.h"
// TODO: Implement this function
BaseScreen::BaseScreen(std::shared_ptr<Process> process, std::string processName) : AConsole(processName), attachedProcess(process)
{
	this->attachedProcess = process;
}
// TODO: Implement this function
void BaseScreen::onEnabled()
{
	system("CLS");
	this->refreshed = false;
	// execute process
	this->display();
}
// TODO: Implement this function
void BaseScreen::process()
{
	if (!this->refreshed)
	{
		this->printProcessInfo();
		this->refreshed = true;
	}

	while (true) {
		// Asking for text input
		std::string sInput;
		std::cout << "Please type in a command: ";
		std::getline(std::cin, sInput);

		// Input validation and conditions
		if (sInput == "process-smi")
		{
			this->printProcessInfo();
		}
		else if (sInput == "clear") {
			system("CLS");
			this->printProcessInfo();
		}
		else if (sInput == "exit") {
			ConsoleManager::getInstance()->unregisterScreen(this->getName());
			ConsoleManager::getInstance()->exitApplication();
			break;
		}
	}

}
// TODO: Implement this function
void BaseScreen::display()
{
		this->process();
}
// TODO: Implement this function
void BaseScreen::printProcessInfo() const
{
	std::cout << "Process: " << this->attachedProcess->getName() << std::endl;
	std::cout << "ID: " << this->attachedProcess->getPID() << std::endl;
	if(this->attachedProcess->isFinished())
	{
		std::cout << "Finished!" << std::endl;
	}
	else
	{
		std::cout << "Current instruction line: " << this->attachedProcess->getCommandCounter() << std::endl;
		std::cout << "Lines of code: " << this->attachedProcess->getLinesOfCode() << std::endl;
	}
	
}

