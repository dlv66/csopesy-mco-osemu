#pragma once
#include <memory>

#include "IThread.h"
#include "Process.h"

class Core : public IThread
{
public:

	// Constructor
	Core(int coreID);

	std::shared_ptr<Process> process;
	std::shared_ptr<Process> terminatedProcess;
	int coreID;

	void update(bool isRunning);

	void run() override;
	void runQuantum(int timeQuantum) override;

	void tick();
	int getCpuCycles() const;

	void setProcess(std::shared_ptr<Process> process);
	bool isRunningBool() const;

private:
	bool isRunning = false;
	bool requiresQuantum = false; // assumes FCFS default scheduler
	int cpuCycles = 0;
};

