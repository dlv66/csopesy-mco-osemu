#pragma once
#include <vector>
#include "Process.h"
#include "Core.h"
#include <iostream>
#include <fstream>


class Initialize
{
public:
	// Constructor
	Initialize();

	// Variables
	std::string myText;
	int numCPU;
	std::string scheduler;
	long long quantumCycles;
	long long batchProcessFreq;
	long long minIns;
	long long maxIns;
	long long delayPerExec;

	// Functions
	void start();
};

