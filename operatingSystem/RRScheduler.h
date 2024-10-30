#pragma once
#include "AScheduler.h"
#include "Process.h"
#include "Core.h"
#include <queue>
#include <memory>
#include <thread>  

// TODO: Implement the round robin scheduler
class RRScheduler : public AScheduler
{
	private:
		std::queue<std::shared_ptr<Process>> processQueue;  // Q of processes
		int timeQuantum;  // quantum in CPU cycles
		int delayPerExec; // delay between instruction executions

	public:
		// Constructor
		RRScheduler(int quantum, int delayExec, int nCores);

		void addProcess(std::shared_ptr<Process> process) override;

		// Instantiates core list based on given number of cores
		void instantiateCoreList();

		// Runs the actual scheduler
		void run() override;
		void init() override;
		void execute() override;
};

