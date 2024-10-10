#pragma once
#include <ctime>
#include <string>

class Process
{
	public:
	    std::string processName;
	    int currentLineOfInstruction;
	    int totalLineOfInstruction;
	    time_t timestamp;
	    struct tm date;
		bool isRunning = false; // if its currently running the commands (true) or not(false)

	    // Default constructor
	    Process() : processName("Unnamed Process"), currentLineOfInstruction(0), totalLineOfInstruction(0) {
	        time(&this->timestamp);
	    }

	    // Custom constructor
	    Process(std::string processName, int currentLineOfInstruction, int totalLineOfInstruction) {
	        this->processName = processName;
	        this->currentLineOfInstruction = currentLineOfInstruction;
	        this->totalLineOfInstruction = totalLineOfInstruction;
	        time(&this->timestamp);
	    }

		std::string		getFormattedDate();
		std::string		getTimePeriod(int hour);
};


