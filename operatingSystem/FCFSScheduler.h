#pragma once
#include <vector>
#include "Process.h"
#include "Core.h"
#include <iostream>

#include "AScheduler.h"


class FCFSScheduler : public AScheduler, public IThread
{
public:
    // Constructor
    FCFSScheduler(int nCores, int delayPerExec);

	// Instantiates core list based on given number of cores
	void instantiateCoreList();

	// Runs the actual scheduler
	void run() override;
	void runQuantum(int timeQuantum) override;
	void init() override;
	void execute() override;
	void executeQuantum(int timeQuantum) override;

	void delay(int coreID);
};

