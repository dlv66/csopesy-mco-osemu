#include "BaseScreen.h"

#include <iostream>
#include <ostream>

#include "MainConsole.h"
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
}
// TODO: Implement this function
void BaseScreen::display()
{
		this->process();
}
// TODO: Implement this function
void BaseScreen::printProcessInfo() const
{
	std::cout << "Process Name: " << this->attachedProcess->getName() << std::endl;
	std::cout << "Process ID: " << this->attachedProcess->getPID() << std::endl;
	//std::cout << "Process Memory: " << this->attachedProcess->getMemory() << std::endl;
}

