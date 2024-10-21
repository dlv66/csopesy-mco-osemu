#include "ConsoleManager.h"

ConsoleManager* ConsoleManager::sharedInstance = nullptr;

ConsoleManager::ConsoleManager()
{
	this->running = true;
	this->consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
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

// TODO: Implement this function
void ConsoleManager::drawConsole() const
{

}

// TODO: Implement this function
void ConsoleManager::process() const
{

}

// TODO: Implement this function
void ConsoleManager::switchConsole(std::string consoleName)
{

}

// TODO: Implement this function
void ConsoleManager::registerScreen(std::shared_ptr<BaseScreen> console)
{

}

// TODO: Implement this function
void ConsoleManager::switchToScreen(std::string screenName)
{

}

// TODO: Implement this function
void ConsoleManager::unregisterScreen(std::string screenName)
{

}

// TODO: Implement this function
void ConsoleManager::returnToPreviousConsole()
{

}

// TODO: Implement this function
void ConsoleManager::exitApplication()
{

}

bool ConsoleManager::isRunning() const
{
	return running;
}
