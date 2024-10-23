#include "ConsoleManager.h"

#include <iostream>

#include "MainConsole.h"

ConsoleManager* ConsoleManager::sharedInstance = nullptr;

ConsoleManager::ConsoleManager()
{
	this->running = true;
	this->consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	const std::shared_ptr<MainConsole> mainConsole = std::make_shared<MainConsole>();
	//const std::shared_ptr<SchedulingConsole> schedulingConsole = std::make_shared<SchedulingConsole>();

	this->consoleTable["MainConsole"] = mainConsole;
	//this->consoleTable["SchedulingConsole"] = schedulingConsole;

	this->switchConsole("MainConsole");
}

ConsoleManager* ConsoleManager::getInstance()
{
	return sharedInstance;
}

void ConsoleManager::initialize()
{
	sharedInstance = new ConsoleManager();
}

void ConsoleManager::destroy()
{
	delete sharedInstance;
}

void ConsoleManager::drawConsole() const
{
	this->currentConsole->display();
	this->currentConsole->process();
}

// TODO: Implement this function
void ConsoleManager::process() const
{

}

void ConsoleManager::switchConsole(std::string consoleName)
{
	if (this->consoleTable.find(consoleName) != this->consoleTable.end())
	{
		// Key exists in the map
		this->previousConsole = this->currentConsole;
		this->currentConsole = this->consoleTable[consoleName];
		this->currentConsole->onEnabled();
	}
	else
	{
		// Key does not exist in the map
		std::cout << "Console " << consoleName << " not found.\n";
	}

}

void ConsoleManager::registerScreen(std::shared_ptr<BaseScreen> screenRef)
{

	if (this->consoleTable.find(screenRef->name) == this->consoleTable.end())
	{
		std::cout << "Screen " << screenRef->name << " registered.\n";
		this->consoleTable[screenRef->name] = screenRef;
		this->switchToScreen(screenRef->name);
	}
	else
	{
		std::cout << "Screen " << screenRef->name << " already exists.\n";
	}
}

void ConsoleManager::registerScreenNoCout(std::shared_ptr<BaseScreen> screenRef) // mostly used for scheduler-test
{

	if (this->consoleTable.find(screenRef->name) == this->consoleTable.end())
	{
		this->consoleTable[screenRef->name] = screenRef;
	}
	else
	{
		std::cout << "Screen " << screenRef->name << " already exists.\n";
	}
}

void ConsoleManager::getScreens()
{
	for (auto const& screen : this->consoleTable)
	{
		std::cout << screen.first << std::endl;
	}
}

void ConsoleManager::switchToScreen(std::string screenName)
{
	if (this->consoleTable.find(screenName) != this->consoleTable.end())
	{
		// Key exists in the map
		this->previousConsole = this->currentConsole;
		this->currentConsole = this->consoleTable[screenName];
		this->currentConsole->onEnabled();
	}
	else
	{
		// Key does not exist in the map
		std::cout << "Screen " << screenName << " not found.\n";
	}
}

// TODO: Implement this function
void ConsoleManager::unregisterScreen(std::string screenName)
{
	this->consoleTable.erase(screenName);
}

// TODO: Implement this function
void ConsoleManager::returnToPreviousConsole()
{
	this->currentConsole = this->previousConsole;
	this->currentConsole->onEnabled();
}

// TODO: Implement this function
void ConsoleManager::exitApplication()
{
	if (this->currentConsole != this->consoleTable["MainConsole"])
	{
		system("CLS");
		this->returnToPreviousConsole();
	}
	else if (this->currentConsole)
	{
		exit(0);
	}
}

bool ConsoleManager::isRunning() const
{
	return running;
}

