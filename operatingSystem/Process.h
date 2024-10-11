#pragma once
#include <ctime>
#include <string>
#include <vector>

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
		int executeTime; // time when the process starts executing
		int finishTime; // time when the process finishes executing

	    std::string processName;
	    int currentLineOfInstruction;
	    int totalLineOfInstruction;
	    time_t timestampCurrent; // current system time
		time_t timestampStart; // time when process started being executed
		time_t timestampFinish; // time when process finished execution
	    struct tm date;
		State state;
		std::vector<std::string> output; // To store printed output


		

		int coreNumber;  // New attribute to store the core number processing this process

		// Default constructor
		Process() : processName("Unnamed Process"), currentLineOfInstruction(0), totalLineOfInstruction(0),
			arrivalTime(0), burstTime(0), coreNumber(-1) {  // Initialize to -1 or some invalid value
			time(&this->timestampCurrent);
		}

		// Custom constructor
		Process(std::string processName, int currentLineOfInstruction, int totalLineOfInstruction,
			int arrival, int burst, int coreNum = -1)
			: processName(processName), currentLineOfInstruction(currentLineOfInstruction),
			totalLineOfInstruction(totalLineOfInstruction), arrivalTime(arrival),
			burstTime(burst), coreNumber(coreNum) {
			time(&this->timestampCurrent);
		}

		/*
	    // Custom constructor
	    Process(std::string processName, int currentLineOfInstruction, int totalLineOfInstruction, int arrival, int burst) {
	        this->processName = processName;
	        this->currentLineOfInstruction = currentLineOfInstruction;
	        this->totalLineOfInstruction = totalLineOfInstruction;
			this->arrivalTime = arrival;
            this->burstTime = burst;
	        time(&this->timestampCurrent);
	    }
		*/
		std::string getTimePeriod(int hour) const;
		std::string getFormattedDate() const;


		void executeCommands();
};


