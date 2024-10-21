#pragma once
#include <string>

class AConsole
{
public:
	typedef std::string String;
	AConsole(String name);
	virtual ~AConsole();

	String getName();
	virtual void onEnabled() = 0; // first time appearance
	virtual void display() = 0; // per frame
	virtual void process() = 0; // input commands or algorithm

	String name;
	friend class ConsoleManager;
};

