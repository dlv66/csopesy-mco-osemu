#pragma once
#include <ctime>
#include <string>

class Process
{
	public:
		// new, running, waiting, ready, terminated
		enum class State {
			NEW, // Process has been created
			RUNNING, // Process is currently running
			WAITING, // Process is waiting for an event to occur
			READY, // Process is ready to be executed and is waiting to be assigned to a core
			TERMINATED // Process is done
		};

		// Attributes for scheduling
		int arrivalTime; // time when the process arrives in the queue   
        int burstTime;	// how long the process will take to execute

	    std::string processName;
	    int currentLineOfInstruction;
	    int totalLineOfInstruction;
	    time_t timestampCurrent; // current system time
		time_t timestampStart; // time when process started being executed
		time_t timestampFinish; // time when process finished execution
	    struct tm date;
		State state = State::NEW;

		

	    // Default constructor
	    Process() : processName("Unnamed Process"), currentLineOfInstruction(0), totalLineOfInstruction(0), arrivalTime(0), burstTime(0) {
	        time(&this->timestampCurrent); // Not sure if timestampCurrent is the correct timestamp to add, but it's a placeholder for now
	    }

	    // Custom constructor
	    Process(std::string processName, int currentLineOfInstruction, int totalLineOfInstruction, int arrival, int burst) {
	        this->processName = processName;
	        this->currentLineOfInstruction = currentLineOfInstruction;
	        this->totalLineOfInstruction = totalLineOfInstruction;
			this->arrivalTime = arrival;
            this->burstTime = burst;
	        time(&this->timestampCurrent);
	    }

		std::string		getFormattedDate();
		std::string		getTimePeriod(int hour);

		void executeCommands();
};


