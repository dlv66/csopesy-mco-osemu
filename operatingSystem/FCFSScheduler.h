#pragma once
#include <vector>
#include "Process.h"
#include <iostream>


class FCFSScheduler
{
	private:
	    int nCores;
	    std::vector<std::vector<Process>> processQueues; // One queue for each core
	public:
	    // Constructor
	    FCFSScheduler(int nCores) {
	        this->nCores = nCores;
	        this->processQueues = std::vector<std::vector<Process>>(nCores);
	    }

	    // Adds a process to the queue
		void addProcess(const Process& process, int nCore = 0);

	    // Sorts process based on remaining instructions
		void sortProcesses();

	    // Runs scheduler
		void runFCFS();
};

