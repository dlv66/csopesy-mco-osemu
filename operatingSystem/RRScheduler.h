#pragma once
#include "AScheduler.h"

// TODO: Implement the round robin scheduler
class RRScheduler : AScheduler
{
public:
	// Constructor
	RRScheduler();

	// Instantiates core list based on given number of cores
	void instantiateCoreList();

	// Runs the actual scheduler
	void run() override;
	void init() override;
	void execute() override;
};

