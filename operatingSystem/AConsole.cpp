#include "AConsole.h"
#include <string>

AConsole::AConsole(String name)
{
	this->name = name;
}

std::string AConsole::getName()
{
	return this->name;
}

AConsole::~AConsole() {
	// Destructor implementation
}