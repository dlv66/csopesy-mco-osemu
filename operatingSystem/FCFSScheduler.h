#pragma once
#include <vector>
#include "Process.h"
#include "Core.h"
#include <iostream>


class FCFSScheduler
{
	public:
	    int nCores;
		std::vector<Core> coreList; // List of cores based on nCores

		std::vector<Process> processQueues; // One queue for each core
		std::vector<Process> activeProcesses; // List of active processes
		std::vector<Process> terminatedProcesses; // List of terminated processes

	    // Constructor
	    FCFSScheduler(int nCores);

		// Instantiates core list based on given number of cores
		void instantiateCoreList();

	    // Adds a process to the queue
		void addProcess(const Process& process);

	    // Sorts process based on remaining instructions
		// void sortProcesses();

	    // Runs scheduler
		void runFCFS();

		// Prints the active processes
		void printActiveProcesses();

		// Prints the finished processes
		void printTerminatedProcesses();
};

