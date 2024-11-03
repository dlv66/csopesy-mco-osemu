#pragma once
#include "AScheduler.h"
#include "Process.h"
#include "Core.h"
#include <queue>
#include <memory>
#include <thread>  

// TODO: Implement the round robin scheduler
class RRScheduler : public AScheduler, public IThread
{
	private:
		std::queue<std::shared_ptr<Process>> processQueue;  // Queue of processes
		long long timeQuantum;  // quantum in CPU cycles
		long long delayPerExec; // delay between instruction executions

	public:
		// Constructor
		RRScheduler(long long quantum, long long delayExec, int nCores);

		// Instantiates core list based on given number of cores
		void instantiateCoreList();

		// Runs the actual scheduler
		//void addProcessRR(std::shared_ptr<Process> process);  // add process to RR queue
		void run() override;
		void runQuantum(long long timeQuantum) override;
		void init() override;
		void execute() override;
		void executeQuantum(long long timeQuantum) override;
};