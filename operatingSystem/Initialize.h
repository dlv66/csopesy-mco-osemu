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
	int quantumCycles;
	int batchProcessFreq;
	int minIns;
	int maxIns;
	int delayPerExec;

	// Functions
	void start();
};

