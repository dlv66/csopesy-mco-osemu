#pragma once
#include <vector>
#include "Process.h"
#include "Core.h"
#include <iostream>

#include "AScheduler.h"


class FCFSScheduler : public AScheduler, public IThread
{
public:
    int nCores;
	std::vector<Core> coreList; // List of cores based on nCores

    // Constructor
    FCFSScheduler(int nCores);

	// Instantiates core list based on given number of cores
	void instantiateCoreList();

    // Adds a process to the queue
	void addProcess(std::shared_ptr<Process> process);

	// Runs the actual scheduler
	void run() override;
	void init() override;
	void execute() override;
};

