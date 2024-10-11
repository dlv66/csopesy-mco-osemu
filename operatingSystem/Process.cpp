#include "Process.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <fstream>
#include <Windows.h>

#include "Utils.h"

std::string Process::getTimePeriod(int hour) {
	return (hour >= 12) ? " PM" : " AM";
}

std::string Process::getFormattedDate() {
	std::string formattedDate;

	if (localtime_s(&this->date, &this->timestampCurrent) != 0) {
		return "Error: Unable to get local time";
	}
	else {
		// (MM/DD/YYYY, HH:MM:SS AM/PM) format
		formattedDate = std::to_string(date.tm_mon + 1) + "/" + std::to_string(date.tm_mday) + "/" + std::to_string(date.tm_year + 1900) + ", " +
			std::to_string(date.tm_hour % 12) + ":" + std::to_string(date.tm_min) + ":" + std::to_string(date.tm_sec) +
			this->getTimePeriod(date.tm_hour);
		return formattedDate;
	}
}

void Process::executeCommands(int coreID) {
	this->state = Process::State::RUNNING;
	this->executeTimet = getCurrentTimestampString();
	time(&this->timestampStart);

	if (this->processName == "EMPTY" || this->processName == "Unnamed Process")
	{
		currentLineOfInstruction = totalLineOfInstruction;
		this->state = Process::State::TERMINATED;
	} 
	else
	{
		std::ofstream outFile(this->processName + ".txt");

		if (!outFile) {
			std::cerr << "Error: Could not open the file for writing." << std::endl;
			return;
		}

		outFile << getCurrentTimestampString() << std::endl;
		outFile << "Process name: " << this->processName << std::endl;
		outFile << "Logs: \n\n" << std::endl;

		for (int i = 0; i < this->totalLineOfInstruction; i++)
		{
			this->currentLineOfInstruction++;
			outFile << "(" << getCurrentTimestampString() << ")" << " Core:" << coreID << " 'Hello world from " << this->processName << "!'" << std::endl;
			Sleep(20);
		}

		outFile.close();
		this->state = Process::State::TERMINATED;

		Sleep(20);
		//once done
	}

		
	
}
