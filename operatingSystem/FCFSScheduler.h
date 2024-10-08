#pragma once
#include <vector>
#include "Process.h"

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


		// NOTE: All functions are defined here but implented in the .cpp file
	    // TODO: Add a process to the queue

	    // TODO: Sort process based on remaining instructions

	    // TODO: Run scheduler
};

