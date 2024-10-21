#include "BaseScreen.h"

#include <iostream>
#include <ostream>
// TODO: Implement this function
BaseScreen::AConsole(std::shared_ptr<Process> process, std::string processName)
{
	this->attachedProcess = process;
}
// TODO: Implement this function
void BaseScreen::onEnabled()
{
	this->refreshed = false;
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
	std::cout << "Process ID: " << this->attachedProcess->getID() << std::endl;
	std::cout << "Process Memory: " << this->attachedProcess->getMemory() << std::endl;
}
