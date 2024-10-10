#pragma once
#include <vector>
#include "Process.h"
#include "Core.h"
#include <iostream>


class FCFSScheduler
{
	private:
	    int nCores;
	    std::vector<Process> processQueues; // One queue for each core
		std::vector<Core> coreList; // List of cores based on nCores

	public:
	    // Constructor
	    FCFSScheduler(int nCores) {
	        this->nCores = nCores;
	        this->processQueues = std::vector<Process>(nCores);
	    }

		// Instantiates core list based on given number of cores
		void instantiateCoreList();

	    // Adds a process to the queue
		void addProcess(const Process& process);

	    // Sorts process based on remaining instructions
		void sortProcesses();

	    // Runs scheduler
		void runFCFS();
};

