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

	void setProcess(std::shared_ptr<Process> process);
private:
	bool isRunning = false;
};

